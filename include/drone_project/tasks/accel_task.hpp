// accel_task.hpp
#pragma once
#include "FreeRTOS.h"
#include <queue.h>

#include <drone_project/types/sensor_data.hpp>

#include "task_wrapper.hpp"
#include <drone_project/drivers/lsm303d_accel.hpp>


namespace drone_project {

class AccelTask : public I2CSensorTask {
private:
    LSM303D_Accel accel_;
    
    bool initialize_sensor();
    void process_accel_data();
    // void handle_read_error();

public:
    explicit AccelTask(QueueHandle_t data_queue);
    
    AccelTask(const AccelTask&) = delete;
    AccelTask& operator=(const AccelTask&) = delete;
    
    void run() override;
    
};

} // namespace drone_project