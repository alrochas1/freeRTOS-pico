// motor_task.hpp
#pragma once
#include "task_wrapper.hpp"
#include "drone_project/drivers/motor_driver.hpp"
#include "drone_project/config/project_config.hpp"

class MotorTask : public Task {
private:
    MotorDriver m1_;
    MotorDriver m2_;
    MotorDriver m3_;
    MotorDriver m4_;

public:
    MotorTask();

protected:
    void run() override;
};