// log_task.cpp
#include "drone_project/tasks/log_task.hpp"
#include "drone_project/config/project_config.hpp"
#include <cstdio>

namespace drone_project {

using namespace config;

LogTask::LogTask(QueueHandle_t gyro_queue, QueueHandle_t accel_queue) 
    : Task("LOG", tasks::LOG_STACK_SIZE, tasks::LOG_PRIORITY)
    , gyro_queue_(gyro_queue), accel_queue_(accel_queue) {
    
    printf("[LOG] Task created \n");
}

void LogTask::run() {
    
    SensorData sensor_data;
    
    while (true) {
        xQueueReceive(gyro_queue_, &sensor_data, 0);
        if (sensor_data.has_gyro()) {
            printf("[LOG] Gyro Data - X: %.2f dps, Y: %.2f dps, Z: %.2f dps, TS: %lu ms, Seq: %lu\n",
                   sensor_data.gyro.angular_velocity.x,
                   sensor_data.gyro.angular_velocity.y,
                   sensor_data.gyro.angular_velocity.z,
                   sensor_data.gyro.timestamp_ms,
                   sensor_data.sequence_number);
        } else {
            printf("[LOG] Invalid gyro data received. Timestamp: %lu ms\n", sensor_data.gyro.timestamp_ms);
        }

        xQueueReceive(accel_queue_, &sensor_data, 0);
        if (sensor_data.has_accel()) {
            printf("[LOG] Accel Data - X: %.2f m/s2, Y: %.2f m/s2, Z: %.2f m/s2, TS: %lu ms, Seq: %lu\n",
                   sensor_data.accel.linear_acceleration.x,
                   sensor_data.accel.linear_acceleration.y,
                   sensor_data.accel.linear_acceleration.z,
                   sensor_data.accel.timestamp_ms,
                   sensor_data.sequence_number);
        } else {
            printf("[LOG] Invalid accel data received. Timestamp: %lu ms\n", sensor_data.accel.timestamp_ms);
        }

        // TODO: Implement mag task (not used in drone_project)
        // xQueueReceive(mag_queue_, &sensor_data, 0); 
        // if (sensor_data.has_mag()) {
        //     printf("[LOG] Mag Data - X: %.2f uT, Y: %.2f uT, Z: %.2f uT, TS: %lu ms, Seq: %lu\n",
        //            sensor_data.mag.magnetic_field.x,
        //            sensor_data.mag.magnetic_field.y,
        //            sensor_data.mag.magnetic_field.z,
        //            sensor_data.mag.timestamp_ms,
        //            sensor_data.sequence_number);
        // } else {
        //     printf("[LOG] Invalid mag data received. Timestamp: %lu ms\n", sensor_data.mag.timestamp_ms);
        // }
            
        delay(tasks::LOG_PRINT_MS);
    }
}

} // namespace drone_project