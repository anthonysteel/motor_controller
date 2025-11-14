#pragma once

#include "lowpass.hpp"

struct SpeedEstimatorConfig {
    LowPassConfig lp;
};

struct SpeedEstimatorState {
    float theta_prev;
    float w_raw;
    LowPassState lp;
    bool initialized;
};

struct SpeedEstimatorInput {
    float theta_meas;
};

struct SpeedEstimatorOutput {
    float w_raw;
    float w_filtered;
};

SpeedEstimatorOutput run_speed_estimator(
    SpeedEstimatorState& state,
    const SpeedEstimatorConfig& cfg,
    const SpeedEstimatorInput& in,
    float dt) noexcept;