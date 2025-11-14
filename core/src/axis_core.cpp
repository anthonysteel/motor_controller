#include "axis_core.hpp"
#include "foc_math.hpp"

AxisCoreOutput run_axis_core(
    AxisCoreState& state,
    const AxisCoreConfig& cfg,
    const AxisCoreInput& in,
    float dt) noexcept
{
    AxisCoreOutput out{};
    out.status = AxisCoreStatus{false, false};

    if (in.v_bus <= 0.0f || dt <= 0.0f) {
        return out;
    }

    SpeedEstimatorInput est_in{in.theta_meas};
    SpeedEstimatorOutput est_out =
        run_speed_estimator(state.est, cfg.est, est_in, dt);

    float w_meas = est_out.w_filtered;

    state.lim.iq_limited = false;
    state.lim.w_limited = false;

    float theta_ref = in.theta_target;
    float w_cmd = 0.0f;
    float iq_cmd = 0.0f;

    switch (in.mode) {
    case AxisMode::Idle:
        return out;

    case AxisMode::CurrentIq:
        iq_cmd = in.iq_target;
        break;

    case AxisMode::Velocity: {
        SpeedLoopInput spd_in{w_meas, in.w_target};
        SpeedLoopOutput spd_out = run_speed_loop(state.spd, cfg.spd, spd_in, dt);
        iq_cmd = spd_out.iq_cmd;
        w_cmd = in.w_target;
    } break;

    case AxisMode::Position: {
        TrajInput traj_in{in.theta_target};
        TrajOutput traj_out = run_traj_step(state.traj, cfg.traj, traj_in, dt);
        theta_ref = traj_out.pos_ref;

        PositionLoopInput pos_in{in.theta_meas, theta_ref};
        PositionLoopOutput pos_out = run_position_loop(state.pos, cfg.pos, pos_in, dt);
        w_cmd = pos_out.w_cmd;

        w_cmd = apply_vel_limit(state.lim, cfg.lim, w_cmd);

        SpeedLoopInput spd_in{w_meas, w_cmd};
        SpeedLoopOutput spd_out = run_speed_loop(state.spd, cfg.spd, spd_in, dt);
        iq_cmd = spd_out.iq_cmd;
    } break;

    default:
        return out;
    }

    iq_cmd = apply_iq_limit(state.lim, cfg.lim, iq_cmd);

    FocInput foc_in{};
    foc_in.i_abc = in.i_abc;
    foc_in.theta_elec = in.theta_elec;
    foc_in.i_setpoint = {0.0f, iq_cmd};
    foc_in.v_bus = in.v_bus;

    FocOutput foc_out =
        run_foc(state.foc, cfg.foc, foc_in, dt);

    ModulationInput mod_in{};
    mod_in.v_ab = foc_out.v_ab;
    mod_in.v_bus = in.v_bus;

    ModulationOutput mod_out = run_modulation(mod_in);

    out.m_a = mod_out.m_a;
    out.m_b = mod_out.m_b;
    out.m_c = mod_out.m_c;
    out.i_dq = foc_out.i_dq;
    out.iq_cmd = iq_cmd;
    out.w_cmd = w_cmd;
    out.theta_ref = theta_ref;
    out.status.iq_limited = state.lim.iq_limited;
    out.status.vel_limited = state.lim.w_limited;

    return out;
}
