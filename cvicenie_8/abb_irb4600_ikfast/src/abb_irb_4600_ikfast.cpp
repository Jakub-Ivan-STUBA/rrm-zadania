#include <abb_irb4600_ikfast/abb_irb4600_ikfast.h>
#include "abb_irb4600_60_205_manipulator_ikfast_solver.cpp"
#include <Eigen/Geometry>

namespace ikfast_abb {

const std::vector<double> joint_min_limits = {-2.62, -1.57, -3.14, -6.98, -2.09, -6.98}; 
const std::vector<double> joint_max_limits = { 2.62,  1.57,  1.31,  6.98,  2.18,  6.98}; 

/**
 * @brief Computes the inverse kinematics (IK) solutions for a given end-effector pose, returning all possible joint configurations within limits.
 *
 * @param pose An Eigen::Affine3d representing the end-effector pose with position and orientation.
 * @return A vector of `JointValues` arrays, where each array represents a valid joint configuration (in radians) for the specified pose.
 */
Solutions computeIK(const Eigen::Affine3d &pose) {
    IkSolutionList<IkReal> solutions;
    const IkReal* pfree = nullptr;  // no redundant joints
    IkReal eerot[9], eetrans[3];

    // Extract position from the pose
    eetrans[0] = pose.translation().x();
    eetrans[1] = pose.translation().y();
    eetrans[2] = pose.translation().z();

    // Extract rotation matrix from the pose
    Eigen::Matrix3d rotation_matrix = pose.rotation();
    eerot[0] = rotation_matrix(0, 0);
    eerot[1] = rotation_matrix(0, 1);
    eerot[2] = rotation_matrix(0, 2);
    eerot[3] = rotation_matrix(1, 0);
    eerot[4] = rotation_matrix(1, 1);
    eerot[5] = rotation_matrix(1, 2);
    eerot[6] = rotation_matrix(2, 0);
    eerot[7] = rotation_matrix(2, 1);
    eerot[8] = rotation_matrix(2, 2);

    // Compute inverse kinematics solutions
    ComputeIk(eetrans, eerot, pfree, solutions);

    // Process and return solutions within joint limits
    Solutions result;
    result.reserve(solutions.GetNumSolutions());
    for (int i = 0; i < solutions.GetNumSolutions(); i++) {
        const auto& sol = solutions.GetSolution(i);
        JointValues jointValues;
        std::vector<IkReal> vsolfree(sol.GetFree().size());
        sol.GetSolution(&jointValues[0], vsolfree.size() > 0 ? &vsolfree[0] : nullptr);

        // Check if each joint in the solution is within the limits
        bool within_limits = true;
        for (size_t j = 0; j < jointValues.size(); ++j) {
            if (jointValues[j] < joint_min_limits[j] || jointValues[j] > joint_max_limits[j]) {
                within_limits = false;
                break;
            }
        }

        // If all joints are within limits, add the solution to the result
        if (within_limits) {
            result.push_back(jointValues);
        }
    }
    return result;
}

/**
 * @brief Computes the forward kinematics (FK) for a given joint configuration, returning the end-effector pose.
 *
 * @param jointValues An array of joint angles (in radians) representing the input joint configuration.
 * @return An Eigen::Affine3d representing the end-effector pose.
 */
Eigen::Affine3d computeFk(const JointValues &jointValues) {
    IkReal eerot[9], eetrans[3];
    ComputeFk(jointValues.data(), eetrans, eerot);

    // Create the transformation matrix from FK results
    Eigen::Affine3d pose = Eigen::Affine3d::Identity();

    // Set position
    pose.translation() << eetrans[0], eetrans[1], eetrans[2];

    // Set rotation matrix
    pose.linear() << eerot[0], eerot[1], eerot[2],
                     eerot[3], eerot[4], eerot[5],
                     eerot[6], eerot[7], eerot[8];

    return pose;
}

} // namespace ikfast_abb