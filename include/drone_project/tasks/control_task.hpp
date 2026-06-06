// control_task.hpp
#pragma once
#include "task_wrapper.hpp"
#include "drone_project/config/project_config.hpp"

#include "drone_project/types/system_data.hpp"
#include "drone_project/types/comms_data.hpp"
#include "drone_project/types/control_data.hpp"


class ControlTask : public Task {
private:
    QueueHandle_t snapshot_queue_;
    QueueHandle_t motor_queue_;

    AttitudeState attitude_;
    DroneController controller_;

    void init_controllers();
    void estimate_state(const SensorData& imu, float dt);
    MotorCommands get_motor_commands(const SystemSnapshot& state);

public:
    ControlTask(QueueHandle_t snapshot_q, QueueHandle_t motor_q);

protected:
    void run() override;
};
