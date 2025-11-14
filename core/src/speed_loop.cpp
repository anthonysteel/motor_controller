#include "speed_loop.hpp"
#include "foc_math.hpp"

SpeedLoopOutput run_speed_loop(
    SpeedLoopState& state,
    const SpeedLoopConfig& cfg,
    const SpeedLoopInput& in,
    float dt) noexcept
{
    SpeedLoopOutput out{};

    float err = in.w_setpoint - in.w_meas;
    float iq = state.iq_pi.update(err, dt);
    iq = clamp(iq, cfg.iq_min, cfg.iq_max);

    out.iq_cmd = iq;
    return out;
}