// gyro_task.cpp
#include "imu_project/tasks/gyro_task.hpp"
#include "imu_project/config/project_config.hpp"
#include <cstdio>

namespace imu_project {

using namespace config;

GyroTask::GyroTask(QueueHandle_t data_queue) 
    : Task("GYRO", tasks::GYRO_STACK_SIZE, tasks::GYRO_PRIORITY)
    , gyro_(i2c1)
    , data_queue_(data_queue) {
    
    printf("[GYRO] Task created. I2C device address: 0x%02X on port %p\n", gyro_.get_address(), (void*)i2c1);
}

bool GyroTask::initialize_gyro() {
    L3GD20::Config gyro_config;
    gyro_config.range = L3GD20::Range::DPS_500;
    gyro_config.output_rate = L3GD20::OutputRate::HZ_95;
    
    if (!gyro_.initialize_config(gyro_config)) {
        printf("[GYRO] FAILED to initialize gyroscope\n");
        return false;
    }
        
    printf("[GYRO] Gyroscope initialized successfully\n");
    return true;
}

void GyroTask::process_gyro_data() {
    auto gyro_data = gyro_.read_gyro();

    SensorData sensor_data;
    sensor_data.gyro = gyro_data;
    sensor_data.sequence_number++;

    // TODO: Add error handling
    xQueueSend(data_queue_, &sensor_data, 0);
    
}

void GyroTask::run() {
    printf("[GYRO] Task started - Sample rate: %lu ms\n", tasks::GYRO_SAMPLE_MS);
    
    if (!initialize_gyro()) {
        printf("[GYRO] WARNING: Starting in degraded mode\n");
    }
    
    while (true) {
        process_gyro_data();
        delay(tasks::GYRO_SAMPLE_MS);
    }
}

} // namespace imu_project