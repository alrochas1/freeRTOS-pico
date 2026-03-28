#pragma once
#include "drone_project/types/sensor_data.hpp"
#include "drone_project/types/comms_data.hpp"

// TODO: Update
enum class SystemState {
    INIT,
    USB,        // System is connected to USB instead of battery (for development/testing)
    DISARMED,
    ARMED,
    FAILSAFE,
    ERROR
};

enum class RunMode {
    FLIGHT,
    IMU_SIM,    // TODO: Implement IMU sim
    RC_SIM,     // TODO: Implement RC sim
    SIMULATION  // TODO: Implement full sim (IMU + RC)
};

// struct SystemConfig {
//     RunMode mode;
// };


struct SystemQueues {
    
    // System queue for system monitor and logging
    QueueHandle_t snapshot_queue;

    // Imput Queues
    QueueHandle_t rc_queue;
    QueueHandle_t gyro_queue;
    QueueHandle_t accel_queue;
        
    QueueHandle_t motor_queue;
};

struct SystemSnapshot {
    SensorData imu;
    RCCommand rc;
    SystemState state;
    uint32_t timestamp_ms;
};


