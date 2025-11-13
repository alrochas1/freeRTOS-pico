#include "imu_project/tasks/led_task.hpp"
#include "imu_project/config/project_config.hpp"
#include <cstdio>

namespace imu_project {

using namespace config;

LedTask::LedTask() 
    : Task("LED", tasks::LED_STACK_SIZE, tasks::LED_PRIORITY)
    , led_pin(pins::LED) {
    
    printf("LED Task created (pin: %d)\n", led_pin);
}

void LedTask::run() {
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    
    printf("LED Task started - Blink interval: %lu ms\n", tasks::LED_BLINK_MS);
    
    while (true) {
        gpio_put(led_pin, state);
        state = !state;
                
        delay(tasks::LED_BLINK_MS);
    }
}

} // namespace imu_project