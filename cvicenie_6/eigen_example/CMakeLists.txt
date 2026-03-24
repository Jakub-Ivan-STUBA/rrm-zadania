cmake_minimum_required(VERSION 3.5)  # ROS2 typically requires at least 3.5
project(eigen_example)

# Find ROS2 packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
  ${EIGEN3_INCLUDE_DIRS}
)

# Add the executable and link against ROS2 libraries
add_executable(eigen_example src/eigen_example.cpp)

ament_target_dependencies(eigen_example "rclcpp")

# Install the target and any launch files
install(TARGETS eigen_example
  DESTINATION lib/${PROJECT_NAME}
)

# Ament export dependencies
ament_package()

