#pragma once
#include <cstdint>
#include "pico/stdlib.h"

namespace config {

// Pins configuration
namespace pins {
    constexpr uint8_t LED = PICO_DEFAULT_LED_PIN;
    constexpr uint8_t USB_PIN = 24; // USB detection pin
    constexpr uint8_t I2C_SDA = 26;
    constexpr uint8_t I2C_SCL = 27;
    // TODO: Add motor pins configuration
    // TODO: Add IR pins configuration
    // TODO: Add RC pins configuration
} // namespace pins

// I2C configuration
namespace i2c {
    constexpr uint32_t BAUD_RATE = 400000; // 400 kHz
    constexpr uint32_t TIMEOUT_MS = 100;
} // namespace i2c

// IMU addresses
namespace imu {
    constexpr uint8_t GYRO_ADDRESS = 0x69;      // L3GD20
    constexpr uint8_t ACCEL_ADDRESS = 0x1E;     // LSM303D
    
    namespace gyro{
        constexpr uint8_t WHO_AM_I_VALUE = 0xD3;   // WHO_AM_I value for L3GD20
        constexpr uint8_t WHO_AM_I_REG = 0x0F;     // WHO_AM_I register for L3GD20
    }
    namespace accel{
        constexpr uint8_t WHO_AM_I_VALUE = 0x3C;   // WHO_AM_I value for LSM303D accel
        constexpr uint8_t WHO_AM_I_REG = 0x0F;     // WHO_AM_I register for LSM303D accel
    }
    namespace mag{
        constexpr uint8_t WHO_AM_I_VALUE = 0xD3;   // WHO_AM_I value for LSM303D mag (revisar)
        constexpr uint8_t WHO_AM_I_REG = 0x0F;     // WHO_AM_I register for LSM303D mag
    }
} // namespace imu

// Task configuration
namespace tasks {
    constexpr uint32_t SYSTEM_STACK_SIZE    = 256;
    constexpr uint32_t LED_STACK_SIZE       = 256;
    constexpr uint32_t LOG_STACK_SIZE       = 1024;
    constexpr uint32_t GYRO_STACK_SIZE      = 1024;
    constexpr uint32_t ACCEL_STACK_SIZE     = 1024;
    constexpr uint32_t IR_STACK_SIZE        = 1024;
    constexpr uint32_t RC_STACK_SIZE        = 1024;
    constexpr uint32_t MOTOR_STACK_SIZE     = 1024;
    
    
    constexpr uint8_t SYSTEM_PRIORITY   = 1;
    constexpr uint8_t LED_PRIORITY      = 1;
    constexpr uint8_t LOG_PRIORITY      = 1;
    constexpr uint8_t GYRO_PRIORITY     = 2;
    constexpr uint8_t ACCEL_PRIORITY    = 2;
    constexpr uint8_t IR_PRIORITY       = 2;
    constexpr uint8_t RC_PRIORITY       = 2;
    constexpr uint8_t MOTOR_PRIORITY    = 2;


    constexpr uint32_t SYSTEM_UPDATE_MS = 50;   // 20 Hz
    constexpr uint32_t LED_BLINK_MS     = 500;  //  2 Hz
    constexpr uint32_t LOG_PRINT_MS     = 500;  //  2 Hz
    constexpr uint32_t GYRO_SAMPLE_MS   = 100;  // 10 Hz
    constexpr uint32_t ACCEL_SAMPLE_MS  = 100;  // 10 Hz
    constexpr uint32_t IR_UPDATE_MS     = 20;   // 50 Hz
    constexpr uint32_t RC_UPDATE_MS     = 20;   // 50 Hz

    // TODO: Add motor task configuration
    // TODO: Add rc task configuration
} // namespace tasks

// Queue configuration
namespace queues {
    constexpr uint8_t SYSTEM_QUEUE_LENGTH = 1;
    constexpr uint8_t SENSOR_QUEUE_LENGTH = 10;
    // constexpr uint8_t MAX_QUEUE_WAIT_MS = portMAX_DELAY;
} // namespace queues

} // namespace config