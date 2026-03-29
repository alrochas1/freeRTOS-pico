// system_task.cpp
#include "drone_project/tasks/system_task.hpp"

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
        state_ = update_state(state_, inputs);
        snap.state = state_;

        xQueueOverwrite(state_queue_, &snap);

        delay(tasks::SYSTEM_UPDATE_MS);
    }
}


SystemState SystemStateTask::update_state(SystemState state, const SystemInputs& in) {

    switch (state) {

    case SystemState::INIT:
        if (in.usb_connected)
            return SystemState::USB;

        if (in.imu_ok)
            return SystemState::DISARMED;

        return SystemState::INIT;

    case SystemState::USB:
        if (!in.usb_connected && in.imu_ok)
            return SystemState::DISARMED;

        return SystemState::USB;

    case SystemState::DISARMED:
        if (!in.imu_ok)
            return SystemState::ERROR;

        if (!in.rc_ok)
            return SystemState::FAILSAFE;

        if (in.arm_switch && in.throttle < 0.05f)
            return SystemState::ARMED;

        return SystemState::DISARMED;

    case SystemState::ARMED:
        if (!in.rc_ok)
            return SystemState::FAILSAFE;

        if (!in.imu_ok)
            return SystemState::ERROR;

        if (in.throttle > 0.1f)
            return SystemState::FLIGHT;

        if (!in.arm_switch)
            return SystemState::DISARMED;

        return SystemState::ARMED;

    case SystemState::FLIGHT:
        if (!in.rc_ok)
            return SystemState::FAILSAFE;

        if (!in.imu_ok)
            return SystemState::ERROR;

        if (in.throttle < 0.05f)
            return SystemState::ARMED;

        return SystemState::FLIGHT;

    case SystemState::FAILSAFE:
        if (in.rc_ok && in.imu_ok)
            return SystemState::DISARMED;

        return SystemState::FAILSAFE;

    case SystemState::ERROR:
        // only manual reset
        return SystemState::ERROR;
    }

    return SystemState::ERROR;
}

