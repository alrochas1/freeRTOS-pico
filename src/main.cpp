// main.cpp
#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "imu_project/tasks/led_task.hpp"


int main() {
    stdio_init_all();
    
    // Wait for the USB serial connection
    sleep_ms(2000);
    printf("Starting IMU Application...\n");
    
    
    LedTask led_task;
    
    // Start tasks
    if (!led_task.start()) {
        printf("Failed to start tasks\n");
        return 1;
    }
    
    printf("All tasks started successfully\n");
    
    // Start scheduler
    vTaskStartScheduler();
    
    // We should never get here
    while (true) {
        tight_loop_contents();
    }
    
    return 0;
}