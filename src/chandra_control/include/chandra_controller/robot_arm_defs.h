#ifndef ROBOT_ARM_DEFS_H
#define ROBOT_ARM_DEFS_H

// I2C device address
#define ROBOT_ARM_I2C_ADDR   0x15

// I2C bus (Raspberry Pi default bus 1)
#define ROBOT_ARM_I2C_BUS    1

// Registers from protocol table
#define REG_READ_VERSION              0x01
#define REG_RGB_LIGHT                 0x02
#define REG_KEY_MODE                  0x03
#define REG_MAIN_CTRL_MODEL           0x04
#define REG_RESTART_MCU               0x05
#define REG_BUZZER                    0x06

// Servo control registers
#define REG_SERVO1_CTRL               0x11
#define REG_SERVO2_CTRL               0x12
#define REG_SERVO3_CTRL               0x13
#define REG_SERVO4_CTRL               0x14
#define REG_SERVO5_CTRL               0x15
#define REG_SERVO6_CTRL               0x16
#define REG_SERVO_ALL_CTRL            0x17

#define REG_SET_SERVO_ID              0x18
#define REG_SPECIFIC_SERVO_POS        0x19
#define REG_SERVO_TORQUE_SWITCH       0x1A
#define REG_NEUTRAL_DEVIATION_STATUS  0x1B
#define REG_SET_NEUTRAL_DEVIATION     0x1C
#define REG_CTRL_6_SERVOS             0x1D
#define REG_CTRL_6_SERVOS_TIME        0x1E

// Action group registers
#define REG_RUN_ACTION_GROUP          0x20
#define REG_VIEW_ACTION_GROUPS        0x22
#define REG_CLEAR_ACTION              0x23

// Servo feedback registers
#define REG_READ_SERVO1_POS           0x31
#define REG_READ_SERVO2_POS           0x32
#define REG_READ_SERVO3_POS           0x33
#define REG_READ_SERVO4_POS           0x34
#define REG_READ_SERVO5_POS           0x35
#define REG_READ_SERVO6_POS           0x36
#define REG_READ_SPECIFIC_SERVO_POS   0x37
#define REG_READ_SERVO_STATE          0x38

// PWM servo control
#define REG_PWM_SERVO1_CTRL           0x51
#define REG_PWM_SERVO2_CTRL           0x52
#define REG_PWM_SERVO3_CTRL           0x53
#define REG_PWM_SERVO4_CTRL           0x54
#define REG_PWM_SERVO5_CTRL           0x55
#define REG_PWM_SERVO6_CTRL           0x56
#define REG_PWM_ALL_SERVOS_CTRL       0x57

// Other constants
#define SERVO_MIN_POS                 96
#define SERVO_MAX_POS                 4000

#endif // ROBOT_ARM_DEFS_H
