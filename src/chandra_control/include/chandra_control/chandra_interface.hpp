#pragma once

#include <hardware_interface/system_interface.hpp>
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <rclcpp/rclcpp.hpp>
#include <vector>
#include "chandra_control/robot_arm_driver.hpp"

namespace chandra_control
{

class ChandraInterface : public hardware_interface::SystemInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(ChandraInterface);

  // Lifecycle methods required by ros2_control
  hardware_interface::CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;
  hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

  // Export state and command interfaces
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  // Read from hardware -> update state
  hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;

  // Write command -> send to hardware
  hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;

private:
  // Conversion helpers
  double ticksToRad(int ticks) const;
  uint16_t radToTicks(double rad) const;

  uint16_t radToServoPos(double angle_rad) const;
  double servoPosToRad(uint16_t pos) const;

  // Driver object
  std::unique_ptr<RobotArmDriver> driver_;

  // ROS2 control state and command vectors
  std::vector<double> hw_states_;
  std::vector<double> hw_commands_;
  std::vector<double> prev_position_commands_;

  // Joint parameters (read from URDF/xacro)
  std::vector<int> servo_ids_;
  // std::vector<double> joint_min_rad_;
  // std::vector<double> joint_max_rad_;
};

}  // namespace chandra_control
