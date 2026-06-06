#pragma once

constexpr float RAD_TO_DEG = 57.2957795131f; // 180 / PI
constexpr float DEG_TO_RAD = 0.01745329251f; // PI / 180

struct PIDController {
    float kp;
    float ki;
    float kd;

    float integral;
    float previous_error;

    float PIDController::update(float error, float dt)
    {
        integral += error * dt;

        float derivative =
            (error - previous_error) / dt;

        previous_error = error;

        return kp * error
            + ki * integral
            + kd * derivative;
    }
};

struct AttitudeState {
    float roll;      // Rotation around X-axis
    float pitch;     // Rotation around Y-axis
    float yaw;       // Rotation around Z-axis
};

// PID controllers for angle and rate control
struct DroneController {
    PIDController angle_roll;
    PIDController angle_pitch;
    PIDController rate_roll;
    PIDController rate_pitch;
    PIDController rate_yaw;
};

struct MotorCommands {
    uint16_t m1;  // Motor 1 power (0-1023)
    uint16_t m2;  // Motor 2 power (0-1023)
    uint16_t m3;  // Motor 3 power (0-1023)
    uint16_t m4;  // Motor 4 power (0-1023)
};