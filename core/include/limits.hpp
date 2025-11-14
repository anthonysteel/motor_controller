#pragma once

struct LimitsConfig {
    float iq_min;
    float iq_max;
    float w_min;
    float w_max;
};

struct LimitsState {
    bool iq_limited;
    bool w_limited;
};

float apply_iq_limit(
    LimitsState& state,
    const LimitsConfig& cfg,
    float iq_cmd) noexcept;

float apply_vel_limit(
    LimitsState& state,
    const LimitsConfig& cfg,
    float w_cmd) noexcept;