// main.cpp
#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "drone_project/tasks/led_task.hpp"
#include "drone_project/tasks/log_task.hpp"

#include "drone_project/tasks/gyro_task.hpp"
#include "drone_project/tasks/accel_task.hpp"
#include "drone_project/tasks/motor_task.hpp"


// For testing individual tasks without the full setup
#define MODE_IMU_TEST   1
#define MODE_MOTOR_TEST 2
#define MODE_FLIGHT     3

#define CURRENT_MODE MODE_MOTOR_TEST

using namespace config;


void common_main() {
    stdio_init_all();

    // Wait for the USB serial connection
    sleep_ms(2500);
    printf("\n=== System Starting ===\n");
    printf("Compiled: %s %s\n", __DATE__, __TIME__);
}


int start_tasks(bool success) {

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


int drone_main() {
    
    common_main();

    // TODO: Implement
    bool success = true; // Placeholder until I implement the actual tasks
    return start_tasks(success);
}


// ########################
// ######### TEST #########
// ########################

int imu_main() {

    common_main();
    
    QueueHandle_t gyro_queue = xQueueCreate(
        queues::SENSOR_QUEUE_LENGTH, 
        sizeof(SensorData)
    );

    QueueHandle_t accel_queue = xQueueCreate(
        queues::SENSOR_QUEUE_LENGTH, 
        sizeof(SensorData)
    );
    
    // TODO: Implement mag task (not used in drone_project)
    // QueueHandle_t mag_queue = xQueueCreate(
    //     queues::SENSOR_QUEUE_LENGTH, 
    //     sizeof(SensorData)
    // );

    if (!gyro_queue || !accel_queue) {  // CHANGE
        printf("ERROR: Failed to create sensor queue\n");
        return 1;
    }
    
    
    LedTask led_task;
    LogTask log_task(gyro_queue, accel_queue);
    GyroTask gyro_task(gyro_queue);
    AccelTask accel_task(accel_queue);
    
    // Start tasks
    bool success = led_task.start() && 
                   gyro_task.start() && 
                   accel_task.start() && 
                   log_task.start();

    return start_tasks(success);
}


int motor_main() {

    common_main();
    
    LedTask led_task;
    MotorTask motor_task(4, 5, 6, 7); // Example GPIOs;

    // Start tasks
    bool success = led_task.start() && 
                   motor_task.start();

    return start_tasks(success);
}


// ########################
// ######### MAIN #########
// ########################

int main() {
#if CURRENT_MODE == MODE_IMU_TEST
    return imu_main();
#elif CURRENT_MODE == MODE_MOTOR_TEST
    return motor_main();
#elif CURRENT_MODE == MODE_FLIGHT
    return drone_main();
#else
    #error "Invalid mode selected"
#endif
}
