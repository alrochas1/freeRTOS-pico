// led_task.hpp
#pragma once
#include "task_wrapper.hpp"
#include "pico/stdlib.h"

class LedTask : public Task {
private:
    uint led_pin;
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
    LedTask() : Task("LED", 256, 1), led_pin(PICO_DEFAULT_LED_PIN) {}
}; 
