#include "foc.hpp"

FocOutput run_foc(
    FocState& state,
    const FocConfig& cfg,
    const FocInput& in,
    float dt
) noexcept 
{
    FocOutput out{};

    AlphaBeta i_ab = clarke(in.i_abc);
    DQ i_dq = park(i_ab, in.theta_elec);

    CurrentLoopInput loop_in{};
    loop_in.i_meas = i_dq;
    loop_in.i_setpoint = in.i_setpoint;
    loop_in.v_bus = in.v_bus;

    CurrentLoopOutput loop_out = 
        run_current_loop(state.loop, cfg.loop, loop_in, dt);
    
    AlphaBeta v_ab = inv_park(loop_out.v_dq, in.theta_elec);

    out.v_ab = v_ab;
    out.i_dq = i_dq;
    return out;
}