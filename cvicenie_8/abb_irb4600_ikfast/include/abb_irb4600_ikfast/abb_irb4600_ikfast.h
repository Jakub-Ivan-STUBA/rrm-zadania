#ifndef ABB_IRB4600_IKFAST_H
#define ABB_IRB4600_IKFAST_H

#include <vector>
#include <array>
#include <functional>
#include <Eigen/Geometry>

namespace ikfast_abb {

    using JointValues = std::array<double, 6>;
    using Solutions = std::vector<JointValues>;

    Solutions computeIK(const Eigen::Affine3d &pose);

    Eigen::Affine3d computeFk(const JointValues &jointValues);

}

#endif //ABB_IRB4600_IKFAST_H
