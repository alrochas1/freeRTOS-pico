// log_task.cpp
#include "imu_project/tasks/log_task.hpp"
#include "imu_project/config/project_config.hpp"
#include <cstdio>

namespace imu_project {

using namespace config;

LogTask::LogTask(QueueHandle_t gyro_queue) 
    : Task("LOG", tasks::LOG_STACK_SIZE, tasks::LOG_PRIORITY)
    , gyro_queue_(gyro_queue) {
    
    printf("[LOG] Task created \n");
}

void LogTask::run() {
    
    SensorData sensor_data;
    
    while (true) {
        xQueueReceive(gyro_queue_, &sensor_data, 0);

        if (sensor_data.is_valid()) {
            printf("[LOG] Gyro Data - X: %.2f dps, Y: %.2f dps, Z: %.2f dps, TS: %lu ms, Seq: %lu\n",
                   sensor_data.gyro.angular_velocity.x,
                   sensor_data.gyro.angular_velocity.y,
                   sensor_data.gyro.angular_velocity.z,
                   sensor_data.gyro.timestamp_ms,
                   sensor_data.sequence_number);
        } else {
            printf("[LOG] Invalid sensor data received\n");
        }
            
        delay(tasks::LOG_PRINT_MS);
    }
}

} // namespace imu_project