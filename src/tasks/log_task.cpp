// log_task.cpp
#include "drone_project/tasks/log_task.hpp"
#include "drone_project/config/project_config.hpp"
#include <cstdio>

using namespace config;

LogTask::LogTask(QueueHandle_t snapshot_queue)
    : Task("LOG", 512, 1),
      snapshot_queue_(snapshot_queue) {

    printf("[LOG] Task created\n");
}

void LogTask::run() {

    SystemSnapshot snap;

    TickType_t last = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(tasks::LOG_PRINT_MS); // 2 Hz logging

    while (true) {

        if (xQueueReceive(snapshot_queue_, &snap, 0) == pdPASS) {

            printf("\n=== SYSTEM SNAPSHOT ===\n");

            printf("[TS] %lu ms\n", snap.timestamp_ms);

            // STATE
            printf("[STATE] %s\n", state_to_string(snap.state));

            // RC
            printf("[RC] Valid:%s T:%.2f R:%.2f P:%.2f Y:%.2f\n",
                   snap.rc.valid ? "OK" : "FAIL",
                   snap.rc.throttle,
                   snap.rc.roll,
                   snap.rc.pitch,
                   snap.rc.yaw);

            // IMU
            printf("[GYRO] %.2f %.2f %.2f\n",
                   snap.imu.gyro.angular_velocity.x,
                   snap.imu.gyro.angular_velocity.y,
                   snap.imu.gyro.angular_velocity.z);

            printf("[ACCEL] %.2f %.2f %.2f\n",
                   snap.imu.accel.linear_acceleration.x,
                   snap.imu.accel.linear_acceleration.y,
                   snap.imu.accel.linear_acceleration.z);
        }

        vTaskDelayUntil(&last, period);
    }
}

const char* LogTask::state_to_string(SystemState state) {
    switch (state) {
        case SystemState::INIT:      return "INIT";
        case SystemState::USB:       return "USB";
        case SystemState::DISARMED:  return "DISARMED";
        case SystemState::ARMED:     return "ARMED";
        case SystemState::FLIGHT:    return "FLIGHT";
        case SystemState::FAILSAFE:  return "FAILSAFE";
        case SystemState::ERROR:     return "ERROR";
        default:                     return "UNKNOWN";
    }
}