// system_task.cpp
#include "drone_project/tasks/system_task.hpp"

#define SIM_BATTERY 0 // For testing without battery (USB mode). Be careful if the motors are connected!

using namespace config;


SystemStateTask::SystemStateTask(QueueHandle_t imu_q,
                             QueueHandle_t rc_q,
                             QueueHandle_t state_q)
    : Task("STATE", 512, 3),
      imu_queue(imu_q),
      rc_queue_(rc_q),
      state_queue_(state_q),
      state_(SystemState::INIT) 
      {
        printf("[SYSTEM STATE] Task created\n");
      }


void SystemStateTask::run() {

    SystemInputs inputs{};
    SensorData imu{};
    RCCommand rc{};
    SystemSnapshot snap{};

    printf("[SYSTEM STATE] Task started - Update interval: %lu ms\n", tasks::SYSTEM_UPDATE_MS);

    while (true) {

        snap.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

        // USB
        inputs.usb_connected = gpio_get(pins::USB_PIN);
        #if SIM_BATTERY
        inputs.usb_connected = false; // TESTING
        #endif

        // IMU
        if (xQueueReceive(imu_queue, &imu, 0) == pdPASS) {
            inputs.imu_ok = imu.has_gyro() && imu.has_accel(); // Add mag if needed
            snap.imu = imu;
        }

        // RC
        if (xQueueReceive(rc_queue_, &rc, 0) == pdPASS) {
            inputs.rc_ok = rc.valid;
            inputs.throttle = rc.throttle;
            inputs.arm_switch = (rc.yaw > 0.8f); // placeholder
            snap.rc = rc;
        } else {
            inputs.rc_ok = false;
        }

        // FSM
        update_state(&state_, inputs);
        snap.state = state_;

        xQueueOverwrite(state_queue_, &snap);

        delay(tasks::SYSTEM_UPDATE_MS);
    }
}


void SystemStateTask::update_state(SystemState* state, const SystemInputs& in) {

    switch (*state) {

    case SystemState::INIT:
        if (in.usb_connected)
            *state = SystemState::USB;

        if (in.imu_ok)  // Add a countdown
            *state = SystemState::DISARMED;

        // Add if more than 30s  without imu, go to error

        *state = SystemState::INIT;

    case SystemState::USB:
        if (!in.usb_connected && in.imu_ok)
            *state = SystemState::DISARMED;

        *state = SystemState::USB;

    case SystemState::DISARMED:
        if (!in.imu_ok) // Add a countdown
            *state = SystemState::ERROR;

        if (!in.rc_ok)  // Add a countdown
            *state = SystemState::FAILSAFE;

        if (in.arm_switch && in.throttle < 0.05f)
            *state = SystemState::ARMED;

        *state = SystemState::DISARMED;

    case SystemState::ARMED:
        if (!in.rc_ok)
            *state = SystemState::FAILSAFE;

        if (!in.imu_ok) // Add a countdown
            *state = SystemState::ERROR;

        if (in.throttle > 0.1f)
            *state = SystemState::FLIGHT;

        if (!in.arm_switch) // Check
            *state = SystemState::DISARMED;

        *state = SystemState::ARMED;

    case SystemState::FLIGHT:
        if (!in.rc_ok)  // Add a countdown
            *state = SystemState::FAILSAFE;

        if (!in.imu_ok) // Add a countdown
            *state = SystemState::ERROR;

        if (in.throttle < 0.05f)    // Add a countdown
            *state = SystemState::ARMED;

        *state = SystemState::FLIGHT;

    case SystemState::FAILSAFE:
        if (in.rc_ok && in.imu_ok)
            *state = SystemState::DISARMED;

        *state = SystemState::FAILSAFE;

    case SystemState::ERROR:
        // only manual reset
        *state = SystemState::ERROR;
    }

    *state = SystemState::ERROR;
}

