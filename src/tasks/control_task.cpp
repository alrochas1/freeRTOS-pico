// control_task.cpp
#include "drone_project/tasks/control_task.hpp"

using namespace config;

ControlTask::ControlTask(QueueHandle_t snapshot_q, QueueHandle_t motor_q)
    : Task("CONTROL", tasks::CONTROL_STACK_SIZE, tasks::CONTROL_PRIORITY),
      snapshot_queue_(snapshot_q),
      motor_queue_(motor_q) {
    
    printf("[CONTROL] Task created\n");

    init_controllers();
}


void ControlTask::init_controllers()
{
    controller_.angle_roll.kp = 4.0f;
    controller_.angle_roll.ki = 0.0f;
    controller_.angle_roll.kd = 0.0f;

    controller_.angle_pitch = controller_.angle_roll;

    controller_.rate_roll.kp = 0.15f;
    controller_.rate_roll.ki = 0.10f;
    controller_.rate_roll.kd = 0.002f;

    controller_.rate_pitch = controller_.rate_roll;

    controller_.rate_yaw.kp = 0.25f;
    controller_.rate_yaw.ki = 0.05f;
    controller_.rate_yaw.kd = 0.0f;
}


void ControlTask::estimate_state(const SensorData& imu, float dt) {

    constexpr float alpha = 0.98f;

    // Simple complementary filter for attitude estimation
    attitude_.roll  += imu.gyro.x * dt;
    attitude_.pitch += imu.gyro.y * dt;
    attitude_.yaw   += imu.gyro.z * dt;

    // Accelerometer-based angle estimation
    float roll_acc = atan2f(
            imu.accel.y,
            imu.accel.z)
            * RAD_TO_DEG;

    float pitch_acc = atan2f(
            -imu.accel.x,
            sqrtf(
                imu.accel.y * imu.accel.y +
                imu.accel.z * imu.accel.z))
            * RAD_TO_DEG;

    // Complementary filter to combine gyro and accel data
    attitude_.roll =
        alpha * attitude_.roll +
        (1.0f - alpha) * roll_acc;

    attitude_.pitch =
        alpha * attitude_.pitch +
        (1.0f - alpha) * pitch_acc;

    attitude_.roll_rate = imu.gyro.x;
    attitude_.pitch_rate = imu.gyro.y;
    attitude_.yaw_rate = imu.gyro.z;
    
}


MotorCommands ControlTask::get_motor_commands(const SystemSnapshot& state) {
    
    // If not armed or in failsafe/error, return zero commands
    if (snapshot.state != SystemState::ARMED &&
        snapshot.state != SystemState::FLIGHT)
    {
        reset_controllers();
        return {};
    }

    // If RC command is invalid, also return zero commands
    if (!snapshot.rc.valid)
    {
        reset_controllers();
        return {};
    }

    return cascaded_control(
        state,
        snapshot.rc);
}


MotorCommands ControlTask::cascaded_control(
    const AttitudeState& state,
    const RCCommand& rc)
{
    constexpr float dt = 0.005f;    // TODO: Fix hardcoded value, use CONTROL_UPDATE_MS instead


    // TODO: Move this constants to config
    constexpr float MAX_ANGLE = 30.0f;
    constexpr float MAX_RATE = 250.0f;
    constexpr float MAX_YAW_RATE = 180.0f;

    // Convert RC stick inputs to target angles and rates
    float target_roll_angle     = rc.roll * MAX_ANGLE;
    float target_pitch_angle    = rc.pitch * MAX_ANGLE;
    float target_yaw_rate       = rc.yaw * MAX_YAW_RATE;


    // Outer loop: Angle control to get target rates
    float target_roll_rate = controller_.angle_roll.update(
            target_roll_angle - state.roll, dt);

    float target_pitch_rate = controller_.angle_pitch.update(
            target_pitch_angle - state.pitch, dt);

    target_roll_rate = std::clamp(
            target_roll_rate, -MAX_RATE, MAX_RATE);

    target_pitch_rate = std::clamp(
            target_pitch_rate, -MAX_RATE, MAX_RATE);


    // Inner loop: Rate control to get motor commands
    float roll_cmd = controller_.rate_roll.update(
            target_roll_rate - state.roll_rate, dt);

    float pitch_cmd = controller_.rate_pitch.update(
            target_pitch_rate - state.pitch_rate, dt);

    float yaw_cmd = controller_.rate_yaw.update(
            target_yaw_rate - state.yaw_rate, dt);

    
    // Scale throttle to motor command range (0-1023)
    float throttle = rc.throttle * 1023.0f;

    float m1 = throttle + pitch_cmd + roll_cmd - yaw_cmd;
    float m2 = throttle + pitch_cmd - roll_cmd + yaw_cmd;
    float m3 = throttle - pitch_cmd - roll_cmd - yaw_cmd;
    float m4 = throttle - pitch_cmd + roll_cmd + yaw_cmd;


    // Clamp motor commands to valid range and convert to uint16_t
    MotorCommands motors;

    auto clamp_motor =
        [](float value)
        {
            return static_cast<uint16_t>(
                std::clamp(
                    value,
                    0.0f,
                    1023.0f));
        };

    motors.m1 = clamp_motor(m1);
    motors.m2 = clamp_motor(m2);
    motors.m3 = clamp_motor(m3);
    motors.m4 = clamp_motor(m4);

    return motors;
}


void ControlTask::run()
{
    constexpr float dt = tasks::CONTROL_UPDATE_MS / 1000.0f;

    SystemSnapshot snapshot{};
    MotorCommands motor_commands{};

    printf("[CONTROL] Task started - Update interval: %lu ms\n", tasks::CONTROL_UPDATE_MS);

    while (true)
    {
        if (xQueuePeek(snapshot_queue_, &snapshot, 0) == pdPASS)
        {
            // Estimate the drone's attitude state from IMU data
            estimate_state(snapshot.imu, dt);

            // Calculate motor commands using PID controller
            motor_commands = get_motor_commands(snapshot);

            // Send motor commands to the motor task
            xQueueOverwrite(motor_queue_, &motor_commands);
        }

        delay(tasks::CONTROL_UPDATE_MS);
    }
}