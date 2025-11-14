#pragma once

#include "foc_math.hpp"
#include "current_loop.hpp"

struct FocConfig {
    CurrentLoopConfig loop;
};

struct FocState {
    CurrentLoopState loop;
};

struct FocInput {
    PhaseCurrents i_abc;
    float theta_elec;
    DQ i_setpoint;
    float v_bus;
};

struct FocOutput {
    AlphaBeta v_ab;
    DQ i_dq;
};

FocOutput run_foc(
    FocState& state,
    const FocConfig& cfg,
    const FocInput& in,
    float dt
) noexcept;