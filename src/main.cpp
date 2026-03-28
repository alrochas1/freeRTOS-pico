// main.cpp
#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "drone_project/config/project_config.hpp"
#include "drone_project/types/system_data.hpp"

#include "drone_project/tasks/system_task.hpp"

#include "drone_project/tasks/led_task.hpp"
#include "drone_project/tasks/log_task.hpp"

#include "drone_project/tasks/gyro_task.hpp"
#include "drone_project/tasks/accel_task.hpp"
//#include "drone_project/tasks/ir_task.hpp"

#include "drone_project/tasks/motor_task.hpp"

// Sim tasks (for testing without hardware)
#include "drone_project/tasks/sim/rc_sim_task.hpp"
// #include "drone_project/tasks/sim/gyro_sim_task.hpp" --- TODO ---


using namespace config;

// Create the different queues used by the system
SystemQueues create_queues() {
    SystemQueues queues;

    // System queue for system monitor and logging
    queues.snapshot_queue = xQueueCreate(queues::SYSTEM_QUEUE_LENGTH, sizeof(SystemSnapshot));

    // Imput Queues
    queues.rc_queue     = xQueueCreate(queues::SYSTEM_QUEUE_LENGTH, sizeof(RCCommand));
    queues.gyro_queue   = xQueueCreate(queues::SENSOR_QUEUE_LENGTH, sizeof(SensorData));
    queues.accel_queue  = xQueueCreate(queues::SENSOR_QUEUE_LENGTH, sizeof(SensorData));
    //queues.mag_queue    = xQueueCreate(queues::SENSOR_QUEUE_LENGTH, sizeof(SensorData));

    // Add error handling for queue creation
    if (!queues.snapshot_queue ||
        !queues.rc_queue ||
        !queues.gyro_queue ||
        !queues.accel_queue) {
    printf("QUEUE INIT FAILED\n");
    while(true);
}
    
    return queues;
}


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

    // For testing individual tasks without the full setup
    RunMode running_mode = RunMode::RC_SIM;
    
    common_main();

    
    // Create queues
    SystemQueues queues = create_queues();


    // Create tasks (TODO: Add structure)
    SystemStateTask system_state_task(queues.gyro_queue, queues.accel_queue, queues.rc_queue, queues.snapshot_queue);
    LedTask led_task;
    LogTask log_task(queues.snapshot_queue);

    // Imput tasks (TODO: Improve this)
    /*if (running_mode == RunMode::IMU_SIM || running_mode == RunMode::SIMULATION) {
        // GyroSimTask gyro_task(queues.gyro_queue);
        // AccelSimTask accel_task(queues.accel_queue);
    } 
    else {*/
        GyroTask gyro_task(queues.gyro_queue);
        AccelTask accel_task(queues.accel_queue);
    // }   

    Task* rc_task = nullptr;
    if (running_mode == RunMode::RC_SIM || running_mode == RunMode::SIMULATION) {
        rc_task = new RCSimTask(queues.rc_queue);
    } /*else {
        rc_task = new IRTask(16, queues.rc_queue);
    }*/

    // ControlTask control_task(queues.rc_queue, queues.gyro_queue, queues.accel_queue, queues.motor_queue);
    MotorTask motor_task(4, 5, 6, 7); // Example GPIOs;


    // TODO: Implement
    // Start tasks
    bool success = led_task.start() && 
                   log_task.start() &&
                   system_state_task.start() &&
                   gyro_task.start() &&
                   accel_task.start() &&
                   rc_task->start() &&
                   motor_task.start();
    return start_tasks(success);
}


// ########################
// ######### MAIN #########
// ########################

int main() {
    return drone_main();
}
