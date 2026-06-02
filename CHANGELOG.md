# Changelog

All notable changes to this project will be documented in this file.


## [0.1.0] - 2026-06-02

### Added

#### Core Infrastructure
- FreeRTOS integration with Raspberry Pi Pico
- CMake-based build system with Pico SDK support
- Task scheduler with configurable stack sizes and priorities
- Queue-based inter-task communication system
- Centralized configuration management

#### Hardware Drivers
- I2C device abstraction layer with error handling
- L3GD20 3-axis gyroscope driver
- LSM303D 3-axis accelerometer driver
- Motor driver with PWM support (4 motor channels)
- USB detection support

#### System Tasks
- **System Task**: Core system monitoring and state management
- **LED Task**: Status indicator control
- **Log Task**: System logging and debug output via USB serial
- **IMU Task**: Inertial Measurement Unit sensor reading and processing
- **Motor Task**: Motor command execution and PWM control
- **Control Task**: Flight control logic and stabilization (not implemented)
- **RC Simulator Task**: Remote control input simulation for testing (no hardware required)
- **IMU Simulator Task**: IMU sensor simulation for testing (no hardware required)


#### Documentation
- README.md with compilation and flashing instructions
- Inline code documentation and comments


### Technical Specifications

#### Supported Hardware
- Raspberry Pi Pico (RP2040)
- L3GD20 Gyroscope (I2C address: 0x69)
- LSM303D Accelerometer (I2C address: 0x1E)
- Generic DC Motors (4 channels PWM support, tested with a 8520 motor and custom drivers)

### Known Limitations

- IR task not yet implemented (commented out in main.cpp)
- RC input pins not yet configured
- Magnetometer support incomplete in LSM303D driver

### Future Work

- [ ] Implement RC receiver input handling
- [ ] Add PID control loops for motor stabilization