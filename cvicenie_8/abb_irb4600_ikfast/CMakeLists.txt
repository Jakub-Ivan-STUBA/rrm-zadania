cmake_minimum_required(VERSION 3.8)
project(abb_irb4600_ikfast)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_compile_options(-Wno-unused-variable)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-sign-compare)
add_compile_options(-Wno-ignored-qualifiers)
add_compile_options(-Wno-unused-but-set-variable)
add_compile_options(-Wno-pedantic)


find_package(ament_cmake REQUIRED)
find_package(Eigen3 REQUIRED)

# Include directories
include_directories(
  ${EIGEN3_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} src/abb_irb_4600_ikfast.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME})

ament_package()
