cmake_minimum_required(VERSION 3.5)
project(abb_model)

find_package(ament_cmake REQUIRED)
find_package(rviz2 REQUIRED)

install(DIRECTORY urdf config launch meshes
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
