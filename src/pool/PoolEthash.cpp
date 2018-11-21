
#include "PoolEthash.h"
#include <src/pool/WorkEthash.h>
#include <src/util/Logging.h>
#include <chrono>
#include <src/network/TcpJsonSubscription.h>
#include <src/common/Json.h>
#include <src/util/HexString.h>
#include <src/pool/AutoRefillQueue.h>
#include <random>
#include <functional>
#include <src/common/Endian.h>
#include <src/algorithm/ethash/Ethash.h>
#include <algorithm> //max

#include <asio.hpp>

namespace miner {

    void PoolEthashStratum::restart() {

        acceptMiningNotify = false;

        auto authorize = std::make_shared<JrpcBuilder>();
        auto subscribe = std::make_unique<JrpcBuilder>();

        subscribe->method("mining.subscribe")
            .param("sgminer")
            .param("5.5.17-gm");

        subscribe->onResponse([this, authorize] (auto &ret) {
            if (!ret.error() && ret.atEquals("result", true))
                jrpc.call(*authorize);
        });

        authorize->method("mining.authorize")
            .param(args.username)
            .param(args.password);

        authorize->onResponse([this] (auto &ret) {
            if (!ret.error())
                acceptMiningNotify = true;
        });

        //set handler for receiving incoming messages that are not responses to sent rpcs
        jrpc.setOnReceive([this] (auto &ret) {
            if (acceptMiningNotify &&
                ret.atEquals("method", "mining.notify")) {
                onMiningNotify(ret.getJson());
            }
            else if (ret.getJson().count("method")) {
                //unsupported method
                jrpc.respond({ret.id(), {JrpcError::method_not_found, "method not supported"}});
            }
            else {
                LOG(INFO) << "unhandled response: " << ret.getJson().dump();
            }
        });

        jrpc.call(*subscribe);
    }

    void PoolEthashStratum::onMiningNotify(const nl::json &j) {
        if (!j.count("params") && j.at("params").size() < 5)
            return;
        auto jparams = j.at("params");

        bool cleanFlag = jparams.at(4);
        if (cleanFlag)
            protocolDatas.clear(); //invalidates all gpu work that links to them

        //create work package
        protocolDatas.emplace_back(std::make_shared<EthashStratumProtocolData>());
        auto &sharedProtoData = protocolDatas.back();
        auto weakProtoData = make_weak(sharedProtoData);

        auto work = std::make_unique<Work<kEthash>>(weakProtoData);

        sharedProtoData->jobId = jparams.at(0);
        HexString(jparams[1]).getBytes(work->header);
        HexString(jparams[2]).getBytes(work->seedHash);
        HexString(jparams[3]).swapByteOrder().getBytes(work->target);

        //work->epoch is calculated in the refill thread

        cleanFlag = true;
        workQueue->setMaster(std::move(work), cleanFlag);
    }

    void PoolEthashStratum::submitWork(unique_ptr<WorkResultBase> resultBase) {

        auto result = static_unique_ptr_cast<WorkResult<kEthash>>(std::move(resultBase));

        //build and send submitMessage on the tcp thread

        jrpc.postAsync([this, result = std::move(result)] {

            auto protoData = result->tryGetProtocolDataAs<EthashStratumProtocolData>();
            if (!protoData) {
                LOG(INFO) << "failed to submit expired work result";
                return; //work has expired
            }

            auto submit = std::make_shared<JrpcBuilder>();

            submit->method("mining.submit")
                .param(args.username)
                .param(protoData.value()->jobId)
                .param("0x" + HexString(toBytesWithBigEndian(result->nonce)).str()) //nonce must be big endian
                .param("0x" + HexString(result->proofOfWorkHash).str())
                .param("0x" + HexString(result->mixHash).str());

            submit->onResponse([] (auto &ret) {
                auto idStr = ret.id() ? std::to_string(ret.id().value()) : "<no id>";
                bool accepted = ret.atEquals("result", true);

                std::string acceptedStr = accepted ? "accepted" : "rejected";
                LOG(INFO) << "share with id " << idStr << " got " << acceptedStr;
            });

            jrpc.callRetryNTimes(*submit, 5, std::chrono::seconds(5), [submit] {
                //this handler gets called if there was no response after the last try
                auto idStr = submit->getId() ? std::to_string(submit->getId().value()) : "<no id>";

                LOG(INFO) << "share with id " << idStr << " got discarded after pool did not respond multiple times";
            });
        });
    }

    PoolEthashStratum::PoolEthashStratum(PoolConstructionArgs args)
    : args(args)
    , jrpc(args.host, args.port) {
        //initialize workQueue
        auto refillThreshold = 8; //once the queue size goes below this, lambda gets called

        auto refillFunc = [] (auto &out, auto &workMaster, size_t currentSize) {

            if (workMaster->epoch == 0) {//calculate epoch for master if it didn't happen yet
                workMaster->epoch = EthCalcEpochNumber(workMaster->seedHash);
            }

            for (auto i = currentSize; i < 16; ++i) {
                ++workMaster->extraNonce;
                auto newWork = std::make_unique<Work<kEthash>>(*workMaster);
                out.push_back(std::move(newWork));
            }
            LOG(INFO) << "workQueue got refilled from " << currentSize << " to " << currentSize + out.size() << " items";
        };

        workQueue = std::make_unique<WorkQueueType>(refillThreshold, refillFunc);

        //jrpc on-restart handler gets called when the connection is first established, and whenever a reconnect happens
        jrpc.setOnRestart([this] {
            restart();
        });
    }

    PoolEthashStratum::~PoolEthashStratum() {
        shutdown = true;
    }

    optional<unique_ptr<WorkBase>> PoolEthashStratum::tryGetWork() {
        if (auto work = workQueue->popWithTimeout(std::chrono::milliseconds(100)))
            return std::move(work.value()); //implicit unique_ptr upcast
        return nullopt; //timeout
    }

    uint64_t PoolEthashStratum::getPoolUid() const {
        return uid;
    }

}
