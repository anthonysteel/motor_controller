#pragma once

#include "pmsm.hpp"
#include "axis_core.hpp"

struct SimAxisConfig {
    AxisCoreConfig axis_cfg;
    PmsmParams motor_params;
    float v_bus;
};

struct SimAxisState {
    AxisCoreState axis_state;
    PmsmState motor_state;
    float theta_mech;
};

void sim_axis_step(
    SimAxisState& st,
    const SimAxisConfig& cfg,
    float dt,
    AxisMode mode,
    float theta_target,
    float w_target,
    float iq_target) noexcept;
