#pragma once

#include "trajectory.hpp"
#include "position_loop.hpp"
#include "speed_loop.hpp"
#include "foc.hpp"
#include "modulation.hpp"

enum class AxisMode {
    Idle,
    CurrentIq,
    Velocity,
    Position
};

struct AxisCoreConfig {
    TrajConfig traj;
    PositionLoopConfig pos;
    SpeedLoopConfig spd;
    CurrentLoopConfig cur;
    FocConfig foc;
};

struct AxisCoreState {
    TrajState traj;
    PositionLoopState pos;
    SpeedLoopState spd;
    FocState foc;
};

struct AxisCoreInput {
    AxisMode mode;
    float theta_meas;
    float w_meas;
    PhaseCurrents i_abc;
    float theta_target;
    float w_target;
    float iq_target;
    float v_bus;
    float theta_elec;
};

struct AxisCoreOutput {
    float m_a;
    float m_b;
    float m_c;
    DQ i_dq;
    float iq_cmd;
    float w_cmd;
    float theta_ref;
};

AxisCoreOutput run_axis_core(
    AxisCoreState& state,
    const AxisCoreConfig& cfg,
    const AxisCoreInput& in,
    float dt) noexcept;