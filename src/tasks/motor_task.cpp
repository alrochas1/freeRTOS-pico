// motor_task.cpp
#include "drone_project/tasks/motor_task.hpp"

using namespace config;

MotorTask::MotorTask(uint m1_pin, uint m2_pin, uint m3_pin, uint m4_pin)
    : Task("MOTOR", 256, 2), // TODO: Adjust stack size and priority
      m1_(m1_pin), m2_(m2_pin), m3_(m3_pin), m4_(m4_pin) {

    m1_.init();
    m2_.init();
    m3_.init();
    m4_.init();

    printf("[MOTOR] Task created \n");
}

void MotorTask::run() {
    // TODO: First test. Change this and change all magic numbers
    const uint16_t m1_power = 750;
    const uint16_t m2_power = 0;
    const uint16_t m3_power = 0;
    const uint16_t m4_power = 0;

    // TODO: Change this to use a queue or something to get the power values from the control task
    while (true) {
        m1_.set_power(m1_power);
        m2_.set_power(m2_power);
        m3_.set_power(m3_power);
        m4_.set_power(m4_power);

        delay(tasks::MOTOR_UPDATE_MS);
    }
}