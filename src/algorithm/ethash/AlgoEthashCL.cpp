
#include "AlgoEthashCL.h"
#include <src/compute/ComputeModule.h>
#include <src/pool/WorkEthash.h>
#include <src/util/Logging.h>
#include <src/util/HexString.h> //for debug logging hex
#include <src/common/Endian.h>
#include <src/compute/opencl/CLProgramLoader.h>
#include <src/common/Future.h>
#include <src/util/StringUtils.h>
#include <memory>
#include <src/compute/opencl/CLError.h>

namespace riner {

    AlgoEthashCL::AlgoEthashCL(AlgoConstructionArgs args)
            : pool(args.workProvider)
            , clProgramLoader(args.compute.getProgramLoaderOpenCL()) {

        VLOG(0) << "EthashCL: launching " << args.assignedDevices.size() << " gpu-tasks";

        for (auto &device : args.assignedDevices) {
            if (auto clDevice = args.compute.getDeviceOpenCL(device.get().id)) {
                size_t i = gpuTasks.size();

                gpuTasks.push_back(std::async(std::launch::async, [=] () {
                    SetThreadNameStream{} << "EthashCL gpu task#" << i;
                    gpuTask(i, *clDevice, device.get());
                }));
            }
        }
    }

    AlgoEthashCL::~AlgoEthashCL() {
        shutdown = true; //set atomic shutdown flag
        //implicitly waits for gpuTasks and submitTasks to finish
    }

    void AlgoEthashCL::gpuTask(size_t taskIndex, cl::Device clDevice, Device &device) {
        const auto &settings = device.settings;
        const unsigned numGpuSubTasks = settings.num_threads;

        //Statistics statistics;
        auto notifyFunc = [] (const char *errinfo, const void *private_info, size_t cb, void *user_data) {
            LOG_N_TIMES(10, ERROR) << errinfo;
        };

        PerPlatform plat;
        plat.clContext = cl::Context(clDevice, nullptr, notifyFunc); //TODO: opencl context should be per-platform

        std::string compilerOptions = MakeStr{} << "-D WORKSIZE=" << settings.work_size;

        auto maybeProgram = clProgramLoader.loadProgram(plat.clContext, "ethash.cl", compilerOptions);
        if (!maybeProgram) {
            LOG(ERROR) << "unable to load ethash kernel, aborting algorithm";
            return;
        }
        VLOG(0) << "successfully compiled ethash kernel";
        plat.clProgram = *maybeProgram;

        DagFile dag;

        VLOG(4) << "trying to get work for dag creation";
        while (!shutdown) {
            //get work only for obtaining dag creation info
            auto work = pool.tryGetWork<WorkEthash>();
            if (!work)
                continue; //check shutdown and try again

            VLOG(4) << "using work to generate dag";

            //~ every 5 days
            {
                VLOG(2) << "waiting for dag cache to be generated";
                typedef decltype(dagCache.readLock()) read_locked_cache_t;
                unique_ptr<read_locked_cache_t> readCachePtr;
                auto lockedCache = dagCache.immediateLock();
                if (!lockedCache->isGenerated(work->epoch)) {
                    auto writeCache = lockedCache.upgrade();
                    writeCache->generate(work->epoch, work->seedHash);
                    readCachePtr = std::make_unique<read_locked_cache_t>(writeCache.downgrade());
                }
                else {
                    readCachePtr = std::make_unique<read_locked_cache_t>(lockedCache.downgrade());
                }
                auto &readCache = *readCachePtr;
                VLOG(0) << "dag cache was generated for epoch " << readCache->getEpoch();

                if (!dag.generate(*readCache, plat.clContext, clDevice, plat.clProgram)) {
                    LOG_N_TIMES(10, ERROR) << "generating dag file failed.";
                    LOG(ERROR) << "aborting algorithm.";
                    shutdown = true;
                    continue;
                }
                VLOG(0) << "successfully generated DagFile";
            }

            VLOG(0) << "launching " << numGpuSubTasks << " gpu-subtasks for the current gpu-task";

            std::vector<std::future<void>> tasks;

            for (size_t i = 0; i < numGpuSubTasks; ++i) {

                tasks.push_back(std::async(std::launch::async, [&, i] () {
                    SetThreadNameStream{} << "EthashCL gpu#" << taskIndex << " subtask#" << i;
                    gpuSubTask(i, plat, clDevice, dag, device);
                }));
            }

            //tasks destructor waits
            //tasks terminate if epoch has changed for every task's work
        }
        VLOG(5) << "gpuTask done";
    }

    void AlgoEthashCL::gpuSubTask(size_t /*subTaskIndex*/, PerPlatform &plat, cl::Device &clDevice, DagFile &dag, Device &device) {
        cl_int err = 0;

        PerGpuSubTask state;
        state.settings = device.settings;

        if (!(state.cmdQueue = cl::CommandQueue(plat.clContext, clDevice, 0, &err))()) {
            LOG_N_TIMES(10, ERROR) << "creating command queue failed in gpuSubTask";
        }
        RNR_RETURN_ON_CL_ERR(err, "create command queue in gpuSubTask",);

        state.clSearchKernel = cl::Kernel(plat.clProgram, "search", &err);
        RNR_RETURN_ON_CL_ERR(err, "unable to create 'search' kernel object from cl program",);

        size_t headerSize = 32;
        state.header = cl::Buffer(plat.clContext, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY,
                headerSize, nullptr, &err);
        RNR_RETURN_ON_CL_ERR(err, "unable to allocate opencl header buffer",);

        state.clOutputBuffer = cl::Buffer(plat.clContext, CL_MEM_READ_WRITE, state.bufferSize, nullptr, &err);
        RNR_RETURN_ON_CL_ERR(err, "unable to allocate opencl output buffer",);

        err = state.cmdQueue.enqueueFillBuffer(state.clOutputBuffer, (uint8_t)0, 0, state.bufferSize);
        RNR_RETURN_ON_CL_ERR(err, "unable to clear opencl output buffer",);

        while (!shutdown) {
            std::shared_ptr<const WorkEthash> work = pool.tryGetWork<WorkEthash>();
            if (!work)
                continue; //check shutdown and try again

            if (work->epoch != dag.getEpoch()) {
                break; //terminate task
            }

            auto raw_intensity = state.settings.raw_intensity;

            for (uint64_t nonce = 0; nonce < UINT32_MAX && !shutdown; nonce += raw_intensity) {

                uint64_t shiftedExtraNonce = uint64_t(work->extraNonce) << 32ULL;

                uint64_t nonceBegin = nonce | shiftedExtraNonce;
                uint64_t nonceEnd = nonceBegin + raw_intensity;

                auto resultNonces = runKernel(state, dag, *work, nonceBegin, nonceEnd);

                for (uint64_t resultNonce : resultNonces) {
                    tasks.addTask([=, &device] () {
                        SetThreadNameStream{} << "EthashCL submit share";
                        submitShare(work, resultNonce, device);
                    });
                }
                device.records.reportScannedNoncesAmount(raw_intensity);

                if (work->expired()) {
                    VLOG(0) << "aborting kernel loop because work has expired on " << getThreadName();
                    break; //get new work
                }
            }
        }
        VLOG(5) << "gpuSubTask done";
    }

    void AlgoEthashCL::submitShare(std::shared_ptr<const WorkEthash> work, uint64_t nonce, Device &device) {

        auto result = work->makeWorkSolution<WorkSolutionEthash>();

        //calculate proof of work hash from nonce and dag-caches
        auto hashes = dagCache.readLock()->getHash(work->header, nonce);

        result->nonce = nonce;
        result->header = work->header;
        result->mixHash = hashes.mixHash;

        if (lessThanLittleEndian(hashes.proofOfWorkHash, work->jobTarget))
            pool.submitSolution(std::move(result));

        bool isValidSolution = lessThanLittleEndian(hashes.proofOfWorkHash, work->deviceTarget);
        device.records.reportWorkUnit(work->deviceDifficulty, isValidSolution);
        if (!isValidSolution) {
            LOG(INFO) << "discarding invalid solution nonce: 0x" << HexString(toBytesWithBigEndian(nonce)).str();
        }
    }

    std::vector<uint64_t> AlgoEthashCL::runKernel(PerGpuSubTask &state, DagFile &dag, const WorkEthash &work,
                                                          uint64_t nonceBegin, uint64_t nonceEnd) {
        cl_int err = 0;
        std::vector<uint64_t> results;

        cl_uint size = dag.getSize();
        cl_uint isolate = UINT32_MAX;
        uint64_t target64 = 0;
        RNR_EXPECTS(work.deviceTarget.size() - 24 == sizeof(target64));
        memcpy(&target64, work.deviceTarget.data() + 24, work.deviceTarget.size() - 24);

        err = state.cmdQueue.enqueueWriteBuffer(state.header, CL_FALSE, 0, work.header.size(), work.header.data());
        RNR_RETURN_ON_CL_ERR(err, "error when writing work header to cl buffer", results);

        cl_uint argI = 0;
        auto &k = state.clSearchKernel;

        k.setArg(argI++, state.clOutputBuffer);
        k.setArg(argI++, state.header);
        k.setArg(argI++, dag.getCLBuffer());
        k.setArg(argI++, size);
        k.setArg(argI++, nonceBegin);
        k.setArg(argI++, target64);
        k.setArg(argI++, isolate);

        cl::NDRange offset{0};

        cl::NDRange localWorkSize{state.settings.work_size};
        cl::NDRange globalWorkSize{state.settings.raw_intensity};

        err = state.cmdQueue.enqueueNDRangeKernel(state.clSearchKernel, offset, globalWorkSize, localWorkSize);
        RNR_CL_ERR(err, "error when enqueueing search kernel");

        cl_bool blocking = CL_FALSE;
        err = state.cmdQueue.enqueueReadBuffer(state.clOutputBuffer, blocking, 0, state.bufferSize, state.outputBuffer.data());
        RNR_RETURN_ON_CL_ERR(err, "error when trying to read back clOutputBuffer after search kernel", results);

        state.cmdQueue.finish();

        //TODO: alternatively for nvidia, use clFlush instead of finish and wait for the cl event of readBuffer command to finish

        auto numFoundNonces = state.outputBuffer.back();
        if (numFoundNonces >= state.bufferCount) {
            LOG_N_TIMES(10, ERROR) << "amount of nonces (" << numFoundNonces << ") in outputBuffer exceeds outputBuffer's size";
        }
        else if (numFoundNonces > 0) {
            //clear the clOutputBuffer by overwriting it with blank
            state.cmdQueue.enqueueFillBuffer(state.clOutputBuffer, (uint8_t)0, state.bufferSize - state.bufferEntrySize, state.bufferEntrySize);

            //populate the result vector and compute the actual 32 bit nonces by adding the outputBuffer contents
            results.resize(numFoundNonces);
            for (size_t i = 0; i < numFoundNonces; ++i) {
                results[i] = nonceBegin + state.outputBuffer[i];
            }
        }

        RNR_EXPECTS(results.size() == numFoundNonces);
        return results;
    }

}
