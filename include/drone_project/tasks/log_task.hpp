// log_task.hpp
#pragma once
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include <queue.h>

#include "task_wrapper.hpp"
#include <drone_project/config/project_config.hpp>
#include <drone_project/types/sensor_data.hpp>
#include "drone_project/types/system_data.hpp"


class LogTask : public Task {
private:
    QueueHandle_t snapshot_queue_;
    
public:
    LogTask(QueueHandle_t snapshot_queue_);

    void run() override;
}; 

