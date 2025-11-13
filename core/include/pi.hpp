#pragma once

struct PI {
    float kp;
    float ki;
    float integral;
    float out_min;
    float out_max;

    float update(float error, float dt) noexcept;
    void reset(float integral_init = 0.0f) noexcept;
};
