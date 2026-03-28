// system_state_task.cpp
#include "task_wrapper.hpp"

#include "drone_project/config/project_config.hpp"
#include "drone_project/types/system_data.hpp"
#include "drone_project/types/comms_data.hpp"

using namespace config;

class SystemStateTask : public Task {
private:

    SystemState update_state(SystemState state, const SystemInputs& in);

    QueueHandle_t gyro_queue_;
    QueueHandle_t accel_queue_;
    QueueHandle_t rc_queue_;
    QueueHandle_t state_queue_;

    SystemState state_;

public:
    SystemStateTask(QueueHandle_t gyro_q,
                    QueueHandle_t accel_q,
                    QueueHandle_t rc_q,
                    QueueHandle_t state_q);

protected:
    void run() override;
};