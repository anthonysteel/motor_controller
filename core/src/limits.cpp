#include "limits.hpp"
#include "foc_math.hpp"

float apply_iq_limit(
    LimitsState& state,
    const LimitsConfig& cfg,
    float iq_cmd) noexcept
{
    float iq = clamp(iq_cmd, cfg.iq_min, cfg.iq_max);
    state.iq_limited = (iq != iq_cmd);
    return iq;
}

float apply_vel_limit(
    LimitsState& state,
    const LimitsConfig& cfg,
    float w_cmd) noexcept
{
    float w = clamp(w_cmd, cfg.w_min, cfg.w_max);
    state.w_limited = (w != w_cmd);
    return w;
}