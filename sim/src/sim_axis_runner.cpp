#include "sim_axis_runner.hpp"
#include "foc_math.hpp"

void sim_axis_step(
    SimAxisState& st,
    const SimAxisConfig& cfg,
    float dt,
    AxisMode mode,
    float theta_target,
    float w_target,
    float iq_target) noexcept
{
    float v_bus = cfg.v_bus;

    float theta_e = st.motor_state.theta_e;
    float p = cfg.motor_params.p;
    float theta_m = wrap_pi(theta_e / p);

    AxisCoreInput in{};
    in.mode = mode;
    in.theta_meas = theta_m;
    in.i_abc = {st.motor_state.ia, st.motor_state.ib, st.motor_state.ic};
    in.theta_target = theta_target;
    in.w_target = w_target;
    in.iq_target = iq_target;
    in.v_bus = v_bus;
    in.theta_elec = theta_e;

    AxisCoreOutput out = run_axis_core(st.axis_state, cfg.axis_cfg, in, dt);

    float half_vbus = 0.5f * v_bus;

    PmsmInput motor_in{};
    motor_in.va = -out.m_a * half_vbus;
    motor_in.vb = -out.m_b * half_vbus;
    motor_in.vc = -out.m_c * half_vbus;
    motor_in.T_L = 0.0f;

    pmsm_step(st.motor_state, cfg.motor_params, motor_in, dt);
}