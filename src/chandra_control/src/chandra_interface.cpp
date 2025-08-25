#include <pluginlib/class_list_macros.hpp>
#include <cmath>
#include <cstdint>
#include "chandra_control/chandra_interface.hpp"
#include "chandra_control/robot_arm_driver.hpp"

namespace chandra_control
{

  // Initialization (parse hardware info from URDF/hardware.yaml)
  hardware_interface::CallbackReturn ChandraInterface::on_init(const hardware_interface::HardwareInfo &info)
  {
    if (SystemInterface::on_init(info) != hardware_interface::CallbackReturn::SUCCESS)
    {
      return hardware_interface::CallbackReturn::ERROR;
    }

    // Setup joint vectors
    size_t num_joints = info.joints.size();
    hw_states_.resize(num_joints, 0.0);
    hw_commands_.resize(num_joints, 0.0);
    prev_position_commands_.resize(num_joints, 0.0);

    servo_ids_.resize(num_joints);
    // joint_min_rad_.resize(num_joints);
    // joint_max_rad_.resize(num_joints);

    // Parse joint configs
    for (size_t i = 0; i < num_joints; i++)
    {
      const auto &joint = info.joints[i];
      servo_ids_[i] = std::stoi(joint.parameters.at("servo_id"));

      // // Joint rad limits
      // joint_min_rad_[i] = info.joints[i].limits.lower;
      // joint_max_rad_[i] = info.joints[i].limits.upper;
    }

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  std::vector<hardware_interface::StateInterface>
  ChandraInterface::export_state_interfaces()
  {
    std::vector<hardware_interface::StateInterface> state_interfaces;
    for (size_t i = 0; i < hw_states_.size(); ++i)
    {
      state_interfaces.emplace_back(
          hardware_interface::StateInterface(
              info_.joints[i].name, "position", &hw_states_[i]));
    }
    return state_interfaces;
  }

  std::vector<hardware_interface::CommandInterface>
  ChandraInterface::export_command_interfaces()
  {
    std::vector<hardware_interface::CommandInterface> command_interfaces;
    for (size_t i = 0; i < hw_commands_.size(); ++i)
    {
      command_interfaces.emplace_back(
          hardware_interface::CommandInterface(
              info_.joints[i].name, "position", &hw_commands_[i]));
    }
    return command_interfaces;
  }

  hardware_interface::CallbackReturn ChandraInterface::on_configure(const rclcpp_lifecycle::State & /*previous_state*/)
  {
    driver_ = std::make_unique<RobotArmDriver>();
    if (!driver_->init())
    {
      RCLCPP_ERROR(rclcpp::get_logger("ChandraInterface"), "Failed to initialize RobotArmDriver");
      return hardware_interface::CallbackReturn::ERROR;
    }

    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn ChandraInterface::on_activate(const rclcpp_lifecycle::State & /*previous_state*/)
  {
    RCLCPP_INFO(rclcpp::get_logger("ChandraInterface"), "Activating ChandraInterface...");
    return hardware_interface::CallbackReturn::SUCCESS;
  }

  hardware_interface::CallbackReturn ChandraInterface::on_deactivate(const rclcpp_lifecycle::State & /*previous_state*/)
  {
    RCLCPP_INFO(rclcpp::get_logger("ChandraInterface"), "Deactivating ChandraInterface...");
    return hardware_interface::CallbackReturn::SUCCESS;
  }

  // Read servo states -> convert to radians
  hardware_interface::return_type ChandraInterface::read(const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
  {

    // Open Loop Control - assuming the robot is always where we command to be
    hw_states_ = hw_commands_;

    // for (size_t i = 0; i < servo_ids_.size(); i++)
    // {
    //   int ticks = driver_->getServoPosition(servo_ids_[i]);
    //   if (ticks >= 0)
    //   {
    //     hw_states_[i] = ticksToRad(ticks);
    //   }
    // }

    return hardware_interface::return_type::OK;
  }

  // Write commanded positions -> convert rad to ticks
  hardware_interface::return_type ChandraInterface::write(const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
  {

    // if (hw_commands_ == prev_position_commands_)
    // {
    //   // Nothing changed, do not send any command
    //   return hardware_interface::return_type::OK;
    // }

    for (size_t i = 0; i < servo_ids_.size(); i++) {
      int ticks = radToTicks(hw_commands_[i]);
      driver_->setServoPosition(servo_ids_[i], ticks, 500);  // default 100ms duration
    }

    // prev_position_commands_ = hw_commands_;
    return hardware_interface::return_type::OK;
  }

  // Convert servo ticks -> radians
  double ChandraInterface::ticksToRad(int ticks) const
  {
    return servoPosToRad(ticks);
  }

  // Convert radians -> servo ticks
  uint16_t ChandraInterface::radToTicks(double rad) const
  {
    return radToServoPos(rad);
  }

  /**
   * @brief Convert radians to Yahboom servo position
   *
   * The servo position range is 96 - 4000, which corresponds
   * to -90° to +90° (i.e., -π/2 to +π/2 radians).
   *
   * @param angle_rad Angle in radians (-π/2 to +π/2)
   * @return uint16_t The equivalent 16-bit servo position
   */
  uint16_t ChandraInterface::radToServoPos(double angle_rad) const
  {
    const double pi = 3.141592653589793;

    // Clamp radians to valid range
    if (angle_rad < -pi / 2)
      angle_rad = -pi / 2;
    if (angle_rad > pi / 2)
      angle_rad = pi / 2;

    // Map radians linearly to servo position
    uint16_t pos = SERVO_MIN_POS + static_cast<uint16_t>((angle_rad + pi / 2) * (SERVO_MAX_POS - SERVO_MIN_POS) / pi);

    return pos;
  }

  /**
   * @brief Convert Yahboom servo position to radians
   *
   * The servo position range is 96 - 4000, which corresponds
   * to -90° to +90° (i.e., -π/2 to +π/2 radians).
   *
   * @param pos The 16-bit servo position value (96 - 4000)
   * @return double The equivalent angle in radians
   */
  double ChandraInterface::servoPosToRad(uint16_t pos) const
  {
    const double pi = 3.141592653589793;

    // Clamp position to valid range
    if (pos < SERVO_MIN_POS)
      pos = SERVO_MIN_POS;
    if (pos > SERVO_MAX_POS)
      pos = SERVO_MAX_POS;

    // Map position linearly to -π/2 to +π/2 radians
    double angle_rad = -pi / 2 + (static_cast<double>(pos - SERVO_MIN_POS) * pi) / (SERVO_MAX_POS - SERVO_MIN_POS);

    return angle_rad;
  }

} // namespace chandra_control

// Export plugin
PLUGINLIB_EXPORT_CLASS(chandra_control::ChandraInterface, hardware_interface::SystemInterface)
