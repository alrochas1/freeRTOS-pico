// log_task.hpp
#pragma once
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include <queue.h>

#include "task_wrapper.hpp"
#include <imu_project/config/project_config.hpp>
#include <imu_project/types/sensor_data.hpp>


namespace imu_project {

class LogTask : public Task {
private:
    QueueHandle_t gyro_queue_;
    QueueHandle_t accel_queue_;
    QueueHandle_t mag_queue_;
    
public:
    LogTask(QueueHandle_t gyro_queue, QueueHandle_t accel_queue, QueueHandle_t mag_queue);

    void run() override;
}; 


} // namespace imu_project
