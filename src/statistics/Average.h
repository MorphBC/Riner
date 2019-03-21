#pragma once

#include <src/common/Chrono.h>
#include <cmath>

namespace miner {

    class ExpAverage {
        const seconds decay_exp;
        double rate{0};
        double weight_rate{0};
        clock::time_point last_update{clock::now()};

        double getExpAvg(double avg, clock::duration delta_t, seconds lambda, double new_value = 0);

    public:
        explicit ExpAverage(seconds decay_exp_) : decay_exp(decay_exp_) {
        }

        void addRecord(double weight, clock::time_point time = clock::now());
        double getRate(clock::time_point time = clock::now());
        double getWeightRate(clock::time_point time = clock::now());
    };

    class Mean {
        uint64_t total{0};
        double weight_total{0};
        double rate{0};
        double weight_rate{0};
        clock::time_point first_update{clock::now()};
        clock::time_point last_update{first_update};

    public:

        clock::duration getElapsedTime(clock::time_point time = clock::now());

        uint64_t getTotal() {
            return total;
        }

        double getTotalWeight() {
            return weight_total;
        }

        void addRecord(double weight, clock::time_point time = clock::now());

        double getRate(clock::time_point time = clock::now());

        double getWeightRate(clock::time_point time = clock::now());

        std::pair<double, double> getAndReset(clock::time_point time = clock::now());
    };

}