// i2c_device.cpp
#include "imu_project/drivers/i2c_device.hpp"
#include "imu_project/config/project_config.hpp"

#include <cstdio>
#include <array>

namespace imu_project {

using namespace config;

I2CDevice::I2CDevice(i2c_inst_t* i2c, uint8_t address) 
    : i2c_port_(i2c), device_address_(address) {
    
    static bool i2c_initialized = false;
    if (!i2c_initialized) {
        i2c_init(i2c_port_, i2c::BAUD_RATE);
        gpio_set_function(pins::I2C_SDA, GPIO_FUNC_I2C);
        gpio_set_function(pins::I2C_SCL, GPIO_FUNC_I2C);
        gpio_pull_up(pins::I2C_SDA);
        gpio_pull_up(pins::I2C_SCL);
        i2c_initialized = true;
        
        printf("I2C initialized at %lu Hz\n", i2c::BAUD_RATE);
    }
}

I2CDevice::Result I2CDevice::write_register(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    
    const int result = i2c_write_blocking(i2c_port_, device_address_, 
                                         buffer, sizeof(buffer), false);
    
    if (result != sizeof(buffer)) {
        printf("I2C write failed: reg=0x%02X, value=0x%02X, result=%d\n", 
               reg, value, result);
        return Result::WRITE_FAILED;
    }
    
    return Result::SUCCESS;
}

I2CDevice::Result I2CDevice::write_registers(uint8_t start_reg, 
                                            const uint8_t* data, 
                                            size_t length) {
    if (data == nullptr || length == 0) {
        return Result::INVALID_PARAM;
    }
    
    std::array<uint8_t, 32> buffer;
    if (length + 1 > buffer.size()) {
        printf("I2C write too large: %zu bytes\n", length);
        return Result::INVALID_PARAM;
    }
    buffer[0] = start_reg;
    
    for (size_t i = 0; i < length; ++i) {
        buffer[i + 1] = data[i];
    }
    
    const int result = i2c_write_blocking(i2c_port_, device_address_, 
                                         buffer.data(), length + 1, false);
    
    
    if (result != static_cast<int>(length + 1)) {
        printf("I2C multi-write failed: start_reg=0x%02X, length=%zu, result=%d\n",
               start_reg, length, result);
        return Result::WRITE_FAILED;
    }
    
    return Result::SUCCESS;
}


// TODO: Error handle
uint8_t I2CDevice::read_register(uint8_t reg) {
    uint8_t value = 0;
    
    if (i2c_write_blocking(i2c_port_, device_address_, &reg, 1, true) != 1) {
        printf("I2C read register failed to write reg: 0x%02X\n", reg);
        return 0;
    }
    
    if (i2c_read_blocking(i2c_port_, device_address_, &value, 1, false) != 1) {
        printf("I2C read register failed to read value for reg: 0x%02X\n", reg);
        return 0;
    }
    
    return value;
}

I2CDevice::Result I2CDevice::read_registers(uint8_t start_reg, 
                                           uint8_t* buffer, 
                                           size_t length) {
    if (buffer == nullptr || length == 0) {
        return Result::INVALID_PARAM;
    }
    
    // Write start register
    if (i2c_write_blocking(i2c_port_, device_address_, &start_reg, 1, true) != 1) {
        printf("I2C multi-read failed to write start reg: 0x%02X\n", start_reg);
        return Result::WRITE_FAILED;
    }
    
    // Read multiple registers
    const int result = i2c_read_blocking(i2c_port_, device_address_, 
                                        buffer, length, false);
    
    if (result != static_cast<int>(length)) {
        printf("I2C multi-read failed: start_reg=0x%02X, length=%zu, result=%d\n",
               start_reg, length, result);
        return Result::READ_FAILED;
    }
    
    return Result::SUCCESS;
}

bool I2CDevice::verify_connection(uint8_t who_am_i_reg, uint8_t expected_id) {
    auto who_am_i = read_register(who_am_i_reg);
    
    if (who_am_i != expected_id) {
        printf("WHO_AM_I mismatch: expected 0x%02X, got 0x%02X\n", 
               expected_id, who_am_i);
        return false;
    }
    
    printf("Device connected: WHO_AM_I = 0x%02X\n", who_am_i);
    return true;
}

} // namespace imu_project