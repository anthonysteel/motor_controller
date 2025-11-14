#include "position_loop.hpp"
#include "foc_math.hpp"

PositionLoopOutput run_position_loop(
    PositionLoopState& state,
    const PositionLoopConfig& cfg,
    const PositionLoopInput& in,
    float dt
) noexcept
{
    PositionLoopOutput out{};

    float err = wrap_pi(in.theta_setpoint - in.theta_meas);
    float w = state.pos_pi.update(err, dt);
    w = clamp(w, cfg.w_min, cfg.w_max);

    out.w_cmd = w;
    return out;
}