// gyro_task.hpp
#pragma once
#include "FreeRTOS.h"
#include <queue.h>

#include <drone_project/types/sensor_data.hpp>

#include "task_wrapper.hpp"
#include <drone_project/drivers/l3gd20.hpp>


namespace drone_project {

class GyroTask : public I2CSensorTask {
private:
    L3GD20 gyro_;
    
    bool initialize_sensor();
    void process_gyro_data();
    // void handle_read_error();

public:
    explicit GyroTask(QueueHandle_t data_queue);
    
    GyroTask(const GyroTask&) = delete;
    GyroTask& operator=(const GyroTask&) = delete;
    
    void run() override;
    
};

} // namespace drone_project