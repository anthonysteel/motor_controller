#pragma once

#include "pi.hpp"

struct PositionLoopConfig {
    float w_min;
    float w_max;
};

struct PositionLoopState {
    PI pos_pi;
};

struct PositionLoopInput {
    float theta_meas;
    float theta_setpoint;
};

struct PositionLoopOutput {
    float w_cmd;
};

PositionLoopOutput run_position_loop(
    PositionLoopState& state,
    const PositionLoopConfig& cfg,
    const PositionLoopInput& in,
    float dt) noexcept;