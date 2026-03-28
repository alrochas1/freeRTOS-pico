// motor_task.cpp
#include "drone_project/tasks/motor_task.hpp"

MotorTask::MotorTask(uint m1_pin, uint m2_pin, uint m3_pin, uint m4_pin)
    : Task("MOTOR", 256, 2), // TODO: Adjust stack size and priority
      m1_(m1_pin), m2_(m2_pin), m3_(m3_pin), m4_(m4_pin) {

    m1_.init();
    m2_.init();
    m3_.init();
    m4_.init();
}

void MotorTask::run() {
    // TODO: First test. Change this snd change all magic numbers
    const uint16_t m1_power = 750;
    const uint16_t m2_power = 0;
    const uint16_t m3_power = 0;
    const uint16_t m4_power = 0;

    TickType_t last = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(10); // 10 ms period (100 Hz update rate)

    // TODO: Change this to use a queue or something to get the power values from the control task
    while (true) {
        m1_.set_power(m1_power);
        m2_.set_power(m2_power);
        m3_.set_power(m3_power);
        m4_.set_power(m4_power);

        vTaskDelayUntil(&last, period);
    }
}