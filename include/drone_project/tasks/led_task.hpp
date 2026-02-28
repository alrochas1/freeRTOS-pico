// led_task.hpp
#pragma once
#include "pico/stdlib.h"

#include "task_wrapper.hpp"
#include <drone_project/config/project_config.hpp>


namespace drone_project {

class LedTask : public Task {
private:
    uint8_t led_pin;
    bool state = false;
    
protected:
    void run() override;
    
public:
    LedTask();
}; 


} // namespace drone_project
