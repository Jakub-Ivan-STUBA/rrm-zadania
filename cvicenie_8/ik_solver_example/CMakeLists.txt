cmake_minimum_required(VERSION 3.8)
project(ik_solver_example)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(abb_irb4600_ikfast REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
  ${EIGEN3_INCLUDE_DIRS}
)

# Executable and linking
add_executable(${PROJECT_NAME} src/ik_solver_example.cpp)
ament_target_dependencies(${PROJECT_NAME} rclcpp abb_irb4600_ikfast)

target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

# Install
install(TARGETS ${PROJECT_NAME}
        DESTINATION lib/${PROJECT_NAME}
        )

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()