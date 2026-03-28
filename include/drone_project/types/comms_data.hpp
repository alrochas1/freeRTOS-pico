#pragma once

struct RCCommand {
    bool  state;      // ON/OFF
    float throttle;
    float roll;
    float pitch;
    float yaw;
};

struct IREdge {
    uint32_t time;
    bool level;
};

