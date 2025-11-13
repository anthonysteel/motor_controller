#pragma once

#include "foc_math.hpp"
#include "pi.hpp"

struct CurrentLoopConfig {
    float mod_radius;
};

struct CurrentLoopState {
    PI id;
    PI iq;
};

struct CurrentLoopInput {
    DQ    i_meas;
    DQ    i_setpoint;
    float v_bus;
};

struct CurrentLoopOutput {
    DQ v_dq;
};

CurrentLoopOutput run_current_loop(
    CurrentLoopState& state,
    const CurrentLoopConfig& cfg,
    const CurrentLoopInput& in,
    float dt) noexcept;
