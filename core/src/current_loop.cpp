#include "current_loop.hpp"

CurrentLoopOutput run_current_loop(
    CurrentLoopState& state,
    const CurrentLoopConfig& cfg,
    const CurrentLoopInput& in,
    float dt) noexcept
{
    CurrentLoopOutput out{};
    if (in.v_bus <= 0.0f || cfg.mod_radius <= 0.0f) {
        out.v_dq = {0.0f, 0.0f};
        return out;
    }

    float err_d = in.i_setpoint.d - in.i_meas.d;
    float err_q = in.i_setpoint.q - in.i_meas.q;
    
    float vd = state.id.update(err_d, dt);
    float vq = state.iq.update(err_q, dt);

    DQ v{vd, vq};
    float v_limit = cfg.mod_radius * in.v_bus;
    saturate(v, v_limit);

    out.v_dq = v;
    return out;
}