#include <rclcpp/rclcpp.hpp>  // ROS2 client library
#include <Eigen/Geometry>     // Eigen library
#include <iostream>           // For using C++ stream output

int main(int argc, char **argv)
{
    // Initialize ROS2
    rclcpp::init(argc, argv);

    // Create a ROS2 node
    auto node = std::make_shared<rclcpp::Node>("eigen_example");

    // Example Eigen operations

    // Initialize vector v1 with elements
    Eigen::VectorXd v1(3);
    v1 << 1, 2, 3;
    RCLCPP_INFO_STREAM(node->get_logger(), "v1 = \n" << v1);

    // Initialize zero vector v2
    Eigen::VectorXd v2 = Eigen::VectorXd::Zero(3);
    RCLCPP_INFO_STREAM(node->get_logger(), "v2 = \n" << v2);

    // Initialize 3x3 matrix m1
    Eigen::MatrixXd m1(3, 3);
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    RCLCPP_INFO_STREAM(node->get_logger(), "m1:\n" << m1);

    // Identity matrix m2
    Eigen::MatrixXd m2 = Eigen::MatrixXd::Identity(3, 3);
    RCLCPP_INFO_STREAM(node->get_logger(), "m2 = \n" << m2);

    // Zero matrix m3
    Eigen::MatrixXd m3 = Eigen::MatrixXd::Zero(3, 3);
    RCLCPP_INFO_STREAM(node->get_logger(), "m3 = \n" << m3);

    // Sum of matrices
    Eigen::MatrixXd sum = m1 + m2;
    RCLCPP_INFO_STREAM(node->get_logger(), "m1 + m2 = \n" << sum);

    // Matrix-vector multiplication
    Eigen::VectorXd product = m1 * v1;
    RCLCPP_INFO_STREAM(node->get_logger(), "m1 * v1 = \n" << product);

    // Inverse of matrix
    Eigen::MatrixXd inverse = m1.inverse();
    RCLCPP_INFO_STREAM(node->get_logger(), "m1^-1 = \n" << inverse);

    // Dot product of vectors
    double dot_product = v1.dot(v2);
    RCLCPP_INFO_STREAM(node->get_logger(), "v1 . v2 = " << dot_product);

    // Cross product of vectors
    Eigen::Vector3d x1 = {1, 2, 3};
    Eigen::Vector3d x2 = {1, 0, 0};
    Eigen::Vector3d cross_product = x1.cross(x2);
    RCLCPP_INFO_STREAM(node->get_logger(), "[1,2,3] x [1,0,0] = \n" << cross_product);

    // Shutdown ROS2
    rclcpp::shutdown();
    return 0;
}
