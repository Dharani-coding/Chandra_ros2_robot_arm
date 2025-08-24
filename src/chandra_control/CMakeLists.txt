cmake_minimum_required(VERSION 3.5)
project(chandra_control)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(trajectory_msgs REQUIRED)
find_package(rclpy REQUIRED)

# -------------------
# Build Robot Arm Driver
# -------------------
add_library(robot_arm_driver SHARED
  src/robot_arm_driver.cpp
)
target_include_directories(robot_arm_driver PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/chandra_controller>
  $<INSTALL_INTERFACE:include>
)

# -------------------
# Build Hardware Interface
# -------------------
add_library(chandra_interface SHARED
  src/chandra_interface.cpp
)
target_include_directories(chandra_interface PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/chandra_controller>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(chandra_interface
  rclcpp
  hardware_interface
  pluginlib
  sensor_msgs
  trajectory_msgs
)
target_link_libraries(chandra_interface
  robot_arm_driver
)

# -------------------
# Build Example Node (slider_control)
# -------------------
add_executable(slider_control src/slider_control.cpp)
target_include_directories(slider_control PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/chandra_controller>
)
ament_target_dependencies(slider_control
  rclcpp
  sensor_msgs
  trajectory_msgs
)
target_link_libraries(slider_control
  chandra_interface
)

# -------------------
# Plugin Export
# -------------------
pluginlib_export_plugin_description_file(hardware_interface chandra_interface.xml)

# -------------------
# Install
# -------------------
install(TARGETS
  robot_arm_driver
  chandra_interface
  slider_control
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
