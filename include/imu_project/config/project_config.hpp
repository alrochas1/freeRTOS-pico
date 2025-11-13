#pragma once
#include <stdint.h>

namespace config {
    constexpr uint32_t LED_TASK_STACK_SIZE = 256;
    constexpr uint32_t LED_TASK_PRIORITY = 1;
    constexpr uint32_t LED_BLINK_MS = 500;
    
    // I2C Configuration
    constexpr uint8_t I2C_SDA_PIN = 4;
    constexpr uint8_t I2C_SCL_PIN = 5;
    constexpr uint32_t I2C_BAUDRATE = 400000;
}

