# Chandra - ROS2 Robot Arm manipulation

A complete ROS 2 robotic arm project for the [Yahboom Dofbot-Pi](http://www.yahboom.net/study/Dofbot-Pi) kit, built with native hardware control, simulation, and MoveIt motion planning.

## Overview

This repository demonstrates a professional ROS 2 robotics stack for a six-joint manipulator with gripper support. It includes:

- A custom `hardware_interface::SystemInterface` plugin for low-level joint control
- An I2C servo driver for Raspberry Pi-based servo actuation
- ROS 2 launch files for real hardware, Gazebo simulation, and MoveIt motion planning
- A MoveIt configuration for real-time inverse kinematics (IK) control in RViz
- A reusable robot description package with URDF/XACRO and RViz visualization files

## Demo


https://github.com/user-attachments/assets/b0235de3-54ac-43cb-a660-ebd9ab1ea763



The demo showcases interactive motion planning using RViz's MoveIt GUI to drag the end effector to target positions, and the arm instantly solves inverse kinematics and executes the motion in real time.


## Project Structure

- `src/chandra_control`
  - `chandra_interface.cpp`: custom ROS 2 hardware interface plugin
  - `robot_arm_driver.cpp`: I2C servo controller and position command driver
  - `slider_control.cpp`: example ROS 2 node publishing joint trajectories
  - `launch/`: hardware and controller launch files
  - `config/chandra_controllers.yaml`: controller configuration for joint and gripper control

- `src/chandra_description`
  - `urdf/`: robot model files for the manipulator
  - `rviz/`: visualization configuration for RViz
  - `meshes/`: custom robot geometry assets

- `src/chandra_moveit`
  - MoveIt 2 configuration and `move_group.launch.py` for motion planning
  - Integration with the robot description and controller packages

- `src/chandra_gazebo`
  - Gazebo world and bridge configuration for simulation-based testing
  - Launch file for spawning the robot and loading controllers in simulation

## Key Capabilities

- Real hardware control of servo joints via `RobotArmDriver`
- Open-loop position write flow from ROS control to physical actuators
- Real-time IK-driven target positioning in RViz through MoveIt
- Simulation-ready launch workflows using Gazebo and ROS-Gazebo bridge

## Skills Demonstrated

- ROS 2 package development
- C++ robotics programming
- hardware interface integration
- motion planning with MoveIt 2
- Gazebo simulation and ROS bridging
- robot modeling with URDF/XACRO
