// motor_task.hpp
#pragma once
#include "task_wrapper.hpp"
#include "drone_project/drivers/motor_driver.hpp"

class MotorTask : public Task {
private:
    MotorDriver m1_;
    MotorDriver m2_;
    MotorDriver m3_;
    MotorDriver m4_;

public:
    MotorTask(uint m1_pin, uint m2_pin, uint m3_pin, uint m4_pin);

protected:
    void run() override;
};