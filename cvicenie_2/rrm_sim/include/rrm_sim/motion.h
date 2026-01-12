#ifndef RRM_SIM_MOTION_H
#define RRM_SIM_MOTION_H

#include <cmath>
#include <thread>
#include <future>
#include <vector>

namespace rrm_sim {

    class SimpleMotor {
    public:
        struct Config {
            double rate; // Communication rate
            double reliability; // Simulate motor failures. 1 - no failure, 0 - no success
        };

        explicit SimpleMotor(const Config &cfg);

        ~SimpleMotor();

        std::future<void> move(double pos, double vel);

        void stop();

        double getCurrentPosition();

        double getCurrentVelocity();

    private:
        std::thread thread;
        std::unique_ptr<std::promise<void>> promise;
        std::mutex mtx;
        bool interrupted{false};

        // Internal state variables
        double position{0.0};
        double velocity{0.0};
        double acceleration{0.0};

        // constants
        const Config config;
        const double threshold{0.001};
    };

    class MotorsChain {
    public:
        MotorsChain(const SimpleMotor::Config &cfg, int size);

        void stop();

        void move(const std::vector<double>& pos, const std::vector<double>& vel);

        std::future<void> move(int id, double pos, double vel);

        std::vector<double> getCurrentPosition();

        std::vector<double> getCurrentVelocity();

    private:
        std::vector<std::shared_ptr<SimpleMotor>> chain;
        std::chrono::milliseconds pullTime{100};
    };

}

#endif //RRM_SIM_MOTION_H
