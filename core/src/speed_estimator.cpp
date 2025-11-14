#include "speed_estimator.hpp"
#include "foc_math.hpp"

SpeedEstimatorOutput run_speed_estimator(
    SpeedEstimatorState& state,
    const SpeedEstimatorConfig& cfg,
    const SpeedEstimatorInput& in,
    float dt) noexcept
{
    SpeedEstimatorOutput out{};

    if (dt <= 0.0f) {
        out.w_raw = state.w_raw;
        out.w_filtered = state.lp.y;
        return out;
    }

    if (!state.initialized) {
        state.theta_prev = in.theta_meas;
        state.w_raw = 0.0f;
        state.lp.y = 0.0f;
        state.lp.initialized = true;
        state.initialized = true;
        out.w_raw = 0.0f;
        out.w_filtered = 0.0f;
        return out;
    }

    float dtheta = wrap_pi(in.theta_meas - state.theta_prev);
    float w = dtheta / dt;

    state.theta_prev = in.theta_meas;
    state.w_raw = w;

    float w_f = lowpass_update(state.lp, cfg.lp, w);

    out.w_raw = w;
    out.w_filtered = w_f;
    return out;
}