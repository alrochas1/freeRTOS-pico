#pragma once
#include <cstdint>
#include "pico/stdlib.h"

namespace config {

// Pins configuration
namespace pins {
    constexpr uint8_t LED = PICO_DEFAULT_LED_PIN;
    constexpr uint8_t I2C_SDA = 26;
    constexpr uint8_t I2C_SCL = 27;
} // namespace pins

// I2C configuration
namespace i2c {
    constexpr uint32_t BAUD_RATE = 400000; // 400 kHz
    constexpr uint32_t TIMEOUT_MS = 100;
} // namespace i2c

// IMU addresses
namespace imu {
    constexpr uint8_t GYRO_ADDRESS = 0x6B;      // L3GD20
    constexpr uint8_t ACCEL_MAG_ADDRESS = 0x1E; // LSM303D
} // namespace imu

// Task configuration
namespace tasks {
    constexpr uint32_t LED_STACK_SIZE = 256;
    constexpr uint32_t GYRO_STACK_SIZE = 1024;
    constexpr uint32_t LOG_STACK_SIZE = 1024;
    
    constexpr uint8_t LED_PRIORITY = 1;
    constexpr uint8_t GYRO_PRIORITY = 2;
    constexpr uint8_t LOG_PRIORITY = 1;
    
    constexpr uint32_t LED_BLINK_MS = 500;
    constexpr uint32_t GYRO_SAMPLE_MS = 100; // 10 Hz
    constexpr uint32_t LOG_PRINT_MS = 100;
} // namespace tasks

// Queue configuration
namespace queues {
    constexpr uint8_t SENSOR_QUEUE_LENGTH = 10;
    // constexpr uint8_t MAX_QUEUE_WAIT_MS = portMAX_DELAY;
} // namespace queues

} // namespace config