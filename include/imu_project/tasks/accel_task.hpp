// accel_task.hpp
#pragma once
#include "FreeRTOS.h"
#include <queue.h>

#include <imu_project/types/sensor_data.hpp>

#include "task_wrapper.hpp"
#include <imu_project/drivers/lsm303d_accel.hpp>


namespace imu_project {

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

} // namespace imu_project