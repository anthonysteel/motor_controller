#pragma once

#include "trajectory.hpp"
#include "position_loop.hpp"
#include "speed_loop.hpp"
#include "foc.hpp"
#include "modulation.hpp"
#include "speed_estimator.hpp"
#include "limits.hpp"

enum class AxisMode {
    Idle,
    CurrentIq,
    Velocity,
    Position,
};

struct AxisCoreStatus {
    bool iq_limited;
    bool vel_limited;
};

struct AxisCoreConfig {
    TrajConfig           traj;
    PositionLoopConfig   pos;
    SpeedLoopConfig      spd;
    CurrentLoopConfig    cur;
    FocConfig            foc;
    SpeedEstimatorConfig est;
    LimitsConfig         lim;
};

struct AxisCoreState {
    TrajState             traj;
    PositionLoopState     pos;
    SpeedLoopState        spd;
    FocState              foc;
    SpeedEstimatorState   est;
    LimitsState           lim;
};

struct AxisCoreInput {
    AxisMode mode;
    float theta_meas;
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
    DQ    i_dq;
    float iq_cmd;
    float w_cmd;
    float theta_ref;
    AxisCoreStatus status;
};

AxisCoreOutput run_axis_core(
    AxisCoreState& state,
    const AxisCoreConfig& cfg,
    const AxisCoreInput& in,
    float dt) noexcept;
