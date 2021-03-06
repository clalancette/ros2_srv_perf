cmake_minimum_required(VERSION 3.8)
project(ros2_srv_perf)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/CheckTime.srv"
)

add_executable(ros2_srv_perf_server
  src/ros2_srv_perf_server.cpp)
ament_target_dependencies(ros2_srv_perf_server
  rclcpp
)
rosidl_target_interfaces(ros2_srv_perf_server ${PROJECT_NAME} "rosidl_typesupport_cpp")

add_executable(ros2_srv_perf_client
  src/ros2_srv_perf_client.cpp)
ament_target_dependencies(ros2_srv_perf_client
  rclcpp
)
rosidl_target_interfaces(ros2_srv_perf_client ${PROJECT_NAME} "rosidl_typesupport_cpp")

install(TARGETS ros2_srv_perf_server ros2_srv_perf_client
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
