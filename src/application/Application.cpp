
#include <iostream>

#include "Application.h"
#include <src/network/TcpJsonSubscription.h>
#include <src/util/FileUtils.h>
#include <src/application/Config.h>
#include <src/util/Logging.h>
#include <src/common/Optional.h>
#include <src/compute/ComputeModule.h>
#include <src/common/Json.h>
#include <src/pool/PoolEthash.h>
#include <src/algorithm/ethash/AlgoEthashCL.h>

#include <thread>
#include <src/algorithm/Algorithm.h>

namespace miner {

    Application::Application(optional<std::string> configPath) {

        if (!configPath) {
            LOG(ERROR) << "no config path command line argument (--config /path/to/config.json)";
            return;
        }

        auto configStr = file::readFileIntoString(configPath.value());
        if (!configStr) {
            LOG(ERROR) << "unable to read config file";
            return;
        }

        LOG(INFO) << "parsing config string:\n" << configStr.value();
        Config config(configStr.value());

        optional_ref<const Config::Pool> configPool;
        config.forEachPool([&] (const Config::Pool &pool) {
            configPool = optional_ref<const Config::Pool>(pool);
        });

        if (!configPool) {
            LOG(ERROR) << "no pool in config";
            return;
        }

        auto &pool = configPool.value();

        auto user = pool.username, password = pool.password;
        LOG(INFO) << "user: " << user << ", password: " << password;

        ComputeModule compute;
        for (auto &id : compute.getAllDeviceIds()) {
            LOG(INFO) << "device id at: " << &id << " name: " << to_string(id.getName());
        };

        auto poolArgs = PoolConstructionArgs {
            "eth-eu1.nanopool.org", "9999", pool.username, pool.password
        };

        PoolEthashStratum poolEthashStratum(poolArgs);

        span<DeviceId> devSpan;

        auto args = AlgoConstructionArgs {
            compute, compute.getAllDeviceIds(), poolEthashStratum
        };

        AlgoEthashCL algo(args);

        for (size_t i = 0; i < 60 * 4; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            //LOG(INFO) << "...";
        }
        LOG(INFO) << "test period over, closing application";

        return;

        auto subscribe = [user, password] (std::ostream &stream) {
            stream << R"({"jsonrpc": "2.0", "method" : "mining.subscribe", "params" : {"username": ")"
                      << user << R"(", "password": ")" << password << R"("}, "id": 1})" << "\n";
        };

        TcpJsonSubscription tcpJson = {"eth-eu1.nanopool.org", "9999", subscribe, [] (auto &json) {

            auto m = json.find("method");
            if (m != json.end() && *m == "mining.notify") {

                LOG(INFO) << "received json: " << json.dump(4);

            }
        }};

        for (size_t i = 0; i < 90; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            LOG(INFO) << "...";
        }
    }

}









