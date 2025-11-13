// gyro_task.hpp
#pragma once
#include "FreeRTOS.h"
#include <queue.h>

#include <imu_project/types/sensor_data.hpp>

#include "task_wrapper.hpp"
#include <imu_project/drivers/l3gd20.hpp>


namespace imu_project {

class GyroTask : public Task {
private:
    L3GD20 gyro_;
    QueueHandle_t data_queue_;
    uint32_t error_count_{0};
    uint32_t success_count_{0};
    
    bool initialize_gyro();
    void process_gyro_data();
    // void handle_read_error();

public:
    explicit GyroTask(QueueHandle_t data_queue);
    
    GyroTask(const GyroTask&) = delete;
    GyroTask& operator=(const GyroTask&) = delete;
    
    void run() override;
    
};

} // namespace imu_project