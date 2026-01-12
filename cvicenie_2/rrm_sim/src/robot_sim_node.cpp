#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "rrm_msgs/msg/command.hpp"
#include "rrm_msgs/srv/command.hpp"
#include "rrm_sim/motion.h"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class RobotSimNode : public rclcpp::Node
{
  public:
    RobotSimNode()
    :
    Node("robot_sim"),
    joint_names_({"joint_1", "joint_2", "joint_3"}),
    robot_({0.05, 1.0}, joint_names_.size())
    {
        publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);

        thread_ = std::thread([this](){
            rclcpp::Rate rate(50ms);
            while (rclcpp::ok()) {
                auto message = sensor_msgs::msg::JointState();
                message.position = robot_.getCurrentPosition();
                message.velocity = robot_.getCurrentVelocity();
                message.name = joint_names_;
                message.header.stamp = this->get_clock()->now();
                publisher_->publish(message);
                rate.sleep();
            }
        });

        subscription_ = this->create_subscription<rrm_msgs::msg::Command>(
                "move_command", 10, std::bind(&RobotSimNode::cmd_callback, this, std::placeholders::_1));

        service_ =
                this->create_service<rrm_msgs::srv::Command>("move_command", std::bind(&RobotSimNode::cmd_service_callback, this, std::placeholders::_1, std::placeholders::_2));
    }

    ~RobotSimNode() override {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

  private:
    std::thread thread_;
    std::vector<std::string> joint_names_;
    rrm_sim::MotorsChain robot_;
    rclcpp::Subscription<rrm_msgs::msg::Command>::SharedPtr subscription_;
    rclcpp::Service<rrm_msgs::srv::Command>::SharedPtr service_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;

    void cmd_service_callback(const std::shared_ptr<rrm_msgs::srv::Command::Request> request,
             std::shared_ptr<rrm_msgs::srv::Command::Response>      response)
    {
        if (request->positions.size() != joint_names_.size()) {
            response->result_code = 1;
            response->message = "Received an incorrect size of desired positions. Command will be skipped";
            RCLCPP_ERROR(this->get_logger(), response->message.c_str());
            return;
        }
        if (request->velocities.size() != joint_names_.size()) {
            response->result_code = 1;
            response->message = "Received an incorrect size of desired velocities. Command will be skipped";
            RCLCPP_ERROR(this->get_logger(), response->message.c_str());
            return;
        }

        try {
            robot_.move(request->positions, request->velocities);
            RCLCPP_INFO(this->get_logger(), "Execution done");
            response->result_code = 0;
            response->message = "Execution done";
        } catch (std::exception &e) {
            response->result_code = 1;
            response->message = "Execution failed: " + std::string(e.what());
            RCLCPP_ERROR(this->get_logger(), response->message.c_str());
        }
    }

    void cmd_callback(const rrm_msgs::msg::Command::SharedPtr msg)
    {
        robot_.move(msg->joint_id, msg->position, 1.0);
    }

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<RobotSimNode>());
  rclcpp::shutdown();
  return 0;
}
