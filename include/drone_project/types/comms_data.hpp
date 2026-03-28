#pragma once

struct RCCommand {
    bool  valid;      // ON/OFF
    float throttle;
    float roll;
    float pitch;
    float yaw;
};

struct IREdge {
    uint32_t time;
    bool level;
};

