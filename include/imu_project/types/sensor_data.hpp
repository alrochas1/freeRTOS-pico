#pragma once
#include <cstdint>
#include <cmath>

namespace imu_project {

struct Vector3f {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
    
    constexpr Vector3f() = default;
    constexpr Vector3f(float x_val, float y_val, float z_val) 
        : x(x_val), y(y_val), z(z_val) {}
    
    bool is_valid() const {
        return !(std::isnan(x) || std::isnan(y) || std::isnan(z));
    }
};

struct GyroData {
    Vector3f angular_velocity; // dps
    uint32_t timestamp_ms{0};
    bool valid{false};
    
    GyroData() = default;
    GyroData(Vector3f velocity, uint32_t ts) 
        : angular_velocity(velocity), timestamp_ms(ts), valid(velocity.is_valid()) {}
};

struct SensorData {
    GyroData gyro;
    // TODO: AccelData accel; MagData mag;
    uint32_t sequence_number{0};
    
    bool is_valid() const {
        return gyro.valid;
    }
};

} // namespace imu_project