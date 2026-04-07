#include "abb_irb4600_ikfast/abb_irb4600_ikfast.h"
#include <rclcpp/rclcpp.hpp>
#include <Eigen/Geometry>

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("ik_solver_example");

    // Define pose
    double x = 0.000872;
    double y = 0.0;
    double z = 2.490139;
    double rx = 0;
    double ry = 0;
    double rz = 0;
    RCLCPP_INFO(node->get_logger(), "Pose x, y, z, rx, ry, rz: %f %f %f %f %f %f", x, y, z, rx, ry, rz);

    Eigen::Affine3d pose = Eigen::Translation3d(Eigen::Vector3d(x,y,z))*
        Eigen::AngleAxisd(rx,Eigen::Vector3d::UnitX())*
        Eigen::AngleAxisd(ry,Eigen::Vector3d::UnitY())*
        Eigen::AngleAxisd(rz,Eigen::Vector3d::UnitZ());

    // Compute IK
    ikfast_abb::Solutions solutions = ikfast_abb::computeIK(pose);
    RCLCPP_INFO(node->get_logger(), "Computed IK for pose with %lu solutions", solutions.size());
    for (const auto &solution : solutions) {
        RCLCPP_INFO(node->get_logger(), "Solution: %f %f %f %f %f %f", solution[0], solution[1], solution[2], solution[3], solution[4], solution[5]);  // Joint angles
    }

    rclcpp::shutdown();
    return 0;
}
