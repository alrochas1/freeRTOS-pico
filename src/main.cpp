// main.cpp
#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "imu_project/tasks/led_task.hpp"
#include "imu_project/tasks/gyro_task.hpp"


int main() {
    
    using namespace imu_project;
    using namespace config;

    stdio_init_all();
    
    // Wait for the USB serial connection
    sleep_ms(2500);
    printf("\n=== System Starting ===\n");
    printf("Compiled: %s %s\n", __DATE__, __TIME__);


    QueueHandle_t sensor_queue = xQueueCreate(
        queues::SENSOR_QUEUE_LENGTH, 
        sizeof(SensorData)
    );
    
    if (!sensor_queue) {
        printf("ERROR: Failed to create sensor queue\n");
        return 1;
    }
    
    
    LedTask led_task;
    GyroTask gyro_task(sensor_queue);
    
    // Start tasks
    bool success = led_task.start() && 
                   gyro_task.start() /*&& 
                   log_task.start()*/;
    
    if (!success) {
        printf("ERROR: Failed to start one or more tasks\n");
        return 1;
    }
    
    printf("All tasks started successfully\n");
    printf("System running...\n");
    
    // Start scheduler
    vTaskStartScheduler();
    
    // We should never get here
    while (true) {
        tight_loop_contents();
    }
    
    return 0;
}