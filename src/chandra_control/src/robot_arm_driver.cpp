#include "chandra_control/robot_arm_driver.hpp"
#include "chandra_control/robot_arm_defs.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Constructor: initialize I2C bus ID and device address, bus_fd_ set to -1 initially
RobotArmDriver::RobotArmDriver(int bus_id, int device_addr)
    : bus_id_(bus_id), device_addr_(device_addr), bus_fd_(-1) {}

// Destructor: close the I2C bus if it was opened
RobotArmDriver::~RobotArmDriver()
{
    if (bus_fd_ >= 0)
        close(bus_fd_);
}

// Initialize the I2C bus and set the slave address
bool RobotArmDriver::init()
{
    // Construct I2C device filename, e.g., /dev/i2c-1
    std::string filename = "/dev/i2c-" + std::to_string(bus_id_);
    bus_fd_ = open(filename.c_str(), O_RDWR); // open I2C bus
    if (bus_fd_ < 0) {
        std::cerr << "Failed to open I2C bus " << filename << std::endl;
        return false;
    }

    // Set the I2C slave address
    if (ioctl(bus_fd_, I2C_SLAVE, device_addr_) < 0) {
        std::cerr << "Failed to set I2C slave address" << std::endl;
        return false;
    }

    return true;
}

// Write bytes to a given register on the servo controller
bool RobotArmDriver::writeBytes(uint8_t reg, const std::vector<uint8_t>& values)
{
    std::vector<uint8_t> buffer;
    buffer.push_back(reg);  // First byte is register address
    buffer.insert(buffer.end(), values.begin(), values.end()); // Append data

    // Perform I2C write
    if (write(bus_fd_, buffer.data(), buffer.size()) != (int)buffer.size()) {
        std::cerr << "I2C write failed for register 0x" << std::hex << (int)reg << std::endl;
        return false;
    }
    return true;
}

// Read a sequence of bytes from a given register
std::vector<uint8_t> RobotArmDriver::readBytes(uint8_t reg, size_t length)
{
    // Write the register address first
    if (write(bus_fd_, &reg, 1) != 1) {
        std::cerr << "I2C write failed (read command)" << std::endl;
        return {};
    }

    // Read back 'length' bytes
    std::vector<uint8_t> buffer(length);
    if (read(bus_fd_, buffer.data(), length) != (int)length) {
        std::cerr << "I2C read failed" << std::endl;
        return {};
    }

    return buffer;
}

// Set position of a single servo with optional movement duration
bool RobotArmDriver::setServoPosition(uint8_t servo_id, uint16_t position, uint16_t duration)
{
    // Clamp the position to valid servo range
    if (position < SERVO_MIN_POS) position = SERVO_MIN_POS;
    if (position > SERVO_MAX_POS) position = SERVO_MAX_POS;

    // Compute register for the servo
    uint8_t reg = REG_SERVO1_CTRL + (servo_id - 1);

    // Pack position and duration into 4 bytes (high byte, low byte for each)
    std::vector<uint8_t> data = {
        static_cast<uint8_t>((position >> 8) & 0xFF),  // High byte of position
        static_cast<uint8_t>(position & 0xFF),         // Low byte of position
        static_cast<uint8_t>((duration >> 8) & 0xFF),  // High byte of duration
        static_cast<uint8_t>(duration & 0xFF)          // Low byte of duration
    };

    // Write data to the servo
    return writeBytes(reg, data);
}

// Read current position of a single servo
int RobotArmDriver::getServoPosition(uint8_t servo_id)
{
    uint8_t reg = REG_READ_SERVO1_POS + (servo_id - 1);

    // Step 1: trigger the servo to report its position
    if (!writeBytes(reg, {0x01})) {
        std::cerr << "Failed to trigger servo position read" << std::endl;
        return -1;
    }

    // Step 2: wait at least 5ms for the servo to update
    usleep(5000);

    // Step 3: read back 2 bytes (high and low byte of position)
    std::vector<uint8_t> data = readBytes(reg, 2);

    // Combine the two bytes into a single integer
    if (data.size() == 2) {
        int pos = (data[0] << 8) | data[1];
        return pos;
    }

    // Return -1 on read failure
    return -1;
}
