#ifndef ROBOT_ARM_DRIVER_HPP
#define ROBOT_ARM_DRIVER_HPP

#include <vector>
#include <string>
#include <cstdint>
#include "robot_arm_defs.h"

class RobotArmDriver
{
public:
    RobotArmDriver(int bus_id = ROBOT_ARM_I2C_BUS, int device_addr = ROBOT_ARM_I2C_ADDR);
    ~RobotArmDriver();

    bool init();
    bool writeBytes(uint8_t reg, const std::vector<uint8_t>& values);
    std::vector<uint8_t> readBytes(uint8_t reg, size_t length);

    // High-level servo control
    bool setServoPosition(uint8_t servo_id, uint16_t position, uint16_t duration = 500);

    // Feedback
    int getServoPosition(uint8_t servo_id);
    

private:
    int bus_id_;
    int device_addr_;
    int bus_fd_;  // I2C bus file descriptor
};

#endif // ROBOT_ARM_DRIVER_HPP
