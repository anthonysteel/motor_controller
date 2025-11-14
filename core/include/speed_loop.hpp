#pragma once

#include "pi.hpp"

struct SpeedLoopConfig {
    float iq_min;
    float iq_max;
};

struct SpeedLoopState {
    PI iq_pi;
};

struct SpeedLoopInput {
    float w_meas;
    float w_setpoint;
};

struct SpeedLoopOutput {
    float iq_cmd;
};

SpeedLoopOutput run_speed_loop(
    SpeedLoopState& state,
    const SpeedLoopConfig& cfg,
    const SpeedLoopInput& in,
    float dt) noexcept;