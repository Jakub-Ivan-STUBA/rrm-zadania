cmake_minimum_required(VERSION 3.5)
project(rrm_simple_robot_model)

find_package(ament_cmake REQUIRED)
find_package(rviz2 REQUIRED)

install(DIRECTORY urdf config launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()

