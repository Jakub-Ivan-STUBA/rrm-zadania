#include <rrm_sim/motion.h>
#include <rclcpp/rclcpp.hpp>

using namespace rrm_sim;

SimpleMotor::SimpleMotor(const SimpleMotor::Config& cfg) : config(cfg) {}

SimpleMotor::~SimpleMotor() {
    interrupted = true;
    if (thread.joinable()) {
        thread.join();
    }
}

std::future<void> SimpleMotor::move(double pos, double vel) {

    if (thread.joinable()) {
        thread.join();
    }
    interrupted = false;
    promise = std::make_unique<std::promise<void>>();
    thread = std::thread([&](double pos, double vel){

        double diff = pos - position;


        double duration = std::abs(position -  pos) / vel;

        if (duration < config.rate) {
            position = pos;
            velocity = 0;
            promise->set_value();
            return;
        }

        double gain = diff / (duration / config.rate);

        rclcpp::WallRate rate(1/ config.rate);
        velocity = vel;

        while (rclcpp::ok() && !interrupted && std::abs(position -  pos) > 0.0001) {
            { // Current position update
                std::lock_guard<std::mutex> lock(mtx);
                if (std::abs(position -  pos) < std::abs(gain)) {
                    position = pos;
                    break;
                }
                position += gain;

                if (static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX) > config.reliability) {
                    // Own exception
                    promise->set_exception(std::make_exception_ptr(std::runtime_error("Motor failed!")));
                    return;
                }
            }
            rate.sleep();
        }
        velocity = 0;
        promise->set_value();

    }, pos, vel);
    return promise->get_future();
}

void SimpleMotor::stop() {
    interrupted = true;
}

double SimpleMotor::getCurrentPosition() {
    std::lock_guard<std::mutex> lock(mtx);
    return position;
}

double SimpleMotor::getCurrentVelocity() {
    std::lock_guard<std::mutex> lock(mtx);
    return velocity;
}


MotorsChain::MotorsChain(const SimpleMotor::Config &cfg, int size) {
    chain.reserve(size);
    for (int i = 0; i < size; i++) {
        chain.push_back(std::make_shared<SimpleMotor>(cfg));
    }
}

void MotorsChain::stop() {
    for (auto & motor : chain) {
        motor->stop();
    }
}

void MotorsChain::move(const std::vector<double>& pos, const std::vector<double>& vel) {
    std::vector<std::future<void>> results;
    std::vector<bool> done(chain.size(), false);
    results.reserve(chain.size());
    for (unsigned int i = 0; i < chain.size(); i++) {
        results.push_back(chain.at(i)->move(pos.at(i), vel.at(i)));
    }

    while (rclcpp::ok()) {
        bool allResultsDone = true;
        for (unsigned int i = 0; i < results.size(); i++){
            if (done.at(i)) {
                continue;
            }
            allResultsDone = false;
            auto status = results.at(i).wait_for(pullTime);
            if (status == std::future_status::ready) {
                results.at(i).get();
                done.at(i) = true;
            }
        }
        if (allResultsDone) {
            break;
        }
    }
}

std::future<void> MotorsChain::move(int id, double pos, double vel) {
    return chain.at(id)->move(pos, vel);
}

std::vector<double> MotorsChain::getCurrentPosition() {
    std::vector<double> positions;
    positions.reserve(chain.size());
    for (auto & motor : chain) {
        positions.push_back(motor->getCurrentPosition());
    }
    return positions;
}

std::vector<double> MotorsChain::getCurrentVelocity() {
    std::vector<double> velocities;
    velocities.reserve(chain.size());
    for (auto & motor : chain) {
        velocities.push_back(motor->getCurrentVelocity());
    }
    return velocities;
}
