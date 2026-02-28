// log_task.hpp
#pragma once
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include <queue.h>

#include "task_wrapper.hpp"
#include <drone_project/config/project_config.hpp>
#include <drone_project/types/sensor_data.hpp>


namespace drone_project {

class LogTask : public Task {
private:
    QueueHandle_t gyro_queue_;
    QueueHandle_t accel_queue_;
    // QueueHandle_t mag_queue_; // TODO: Implement mag task (not used in drone_project)
    
public:
    LogTask(QueueHandle_t gyro_queue, QueueHandle_t accel_queue);

    void run() override;
}; 


} // namespace drone_project
