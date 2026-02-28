// accel_task.cpp
#include "imu_project/tasks/accel_task.hpp"
#include "imu_project/config/project_config.hpp"
#include <cstdio>

namespace imu_project {

using namespace config;

AccelTask::AccelTask(QueueHandle_t data_queue) 
    : I2CSensorTask("ACCEL", tasks::ACCEL_STACK_SIZE, tasks::ACCEL_PRIORITY, data_queue)
    , accel_(i2c1) {
    
    printf("[ACCEL] Task created. I2C device address: 0x%02X on port %p\n", accel_.get_address(), (void*)i2c1);
}

bool AccelTask::initialize_sensor() {
    LSM303D_Accel::Config accel_config;
    accel_config.range = LSM303D_Accel::Range::G_2;
    accel_config.output_rate = LSM303D_Accel::OutputRate::HZ_100;
    
    if (!accel_.initialize_config(accel_config)) {
        printf("[ACCEL] FAILED to initialize accelerometer\n");
        return false;
    }
        
    printf("[ACCEL] Accelerometer initialized successfully\n");
    return true;
}

void AccelTask::process_accel_data() {
    auto accel_data = accel_.read_accel();

    SensorData sensor_data;
    sensor_data.accel = accel_data;
    sensor_data.sequence_number++;

    // TODO: Add error handling
    xQueueSend(data_queue_, &sensor_data, 0);
    
}

void AccelTask::run() {
    printf("[ACCEL] Task started - Sample rate: %lu ms\n", tasks::ACCEL_SAMPLE_MS);
    
    if (!initialize_sensor()) {
        printf("[ACCEL] WARNING: Starting in degraded mode\n");
    }
    
    while (true) {
        process_accel_data();
        delay(tasks::ACCEL_SAMPLE_MS);
    }
}

} // namespace imu_project