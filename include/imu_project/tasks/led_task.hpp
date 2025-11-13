// led_task.hpp
#pragma once
#include "pico/stdlib.h"

#include "task_wrapper.hpp"
#include <imu_project/config/project_config.hpp>


class LedTask : public Task {
private:
    uint8_t led_pin;
    bool state = false;
    
protected:
    void run() override {
        gpio_init(led_pin);
        gpio_set_dir(led_pin, GPIO_OUT);
        
        while (true) {
            gpio_put(led_pin, state);
            state = !state;
            delay(250);
        }
    }
    
public:
    LedTask() : Task("LED", config::LED_TASK_STACK_SIZE, config::LED_TASK_PRIORITY), led_pin(PICO_DEFAULT_LED_PIN) {}
}; 
