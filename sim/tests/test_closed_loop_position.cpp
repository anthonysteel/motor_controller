#include <gtest/gtest.h>
#include <cmath>
#include "sim_axis_runner.hpp"
#include "foc_math.hpp"

static SimAxisConfig make_sim_axis_cfg() {
    SimAxisConfig cfg{};

    cfg.axis_cfg.traj = TrajConfig{1.0f, 2.0f};
    cfg.axis_cfg.pos  = PositionLoopConfig{-50.0f, 50.0f};
    cfg.axis_cfg.spd  = SpeedLoopConfig{-50.0f, 50.0f};
    cfg.axis_cfg.cur  = CurrentLoopConfig{0.8f};
    cfg.axis_cfg.foc  = FocConfig{cfg.axis_cfg.cur};
    cfg.axis_cfg.est  = SpeedEstimatorConfig{LowPassConfig{0.2f}};
    cfg.axis_cfg.lim  = LimitsConfig{-50.0f, 50.0f, -50.0f, 50.0f};

    cfg.motor_params.Rs = 0.1f;
    cfg.motor_params.Ls = 0.001f;
    cfg.motor_params.psi_m = 0.05f;
    cfg.motor_params.p = 4.0f;
    cfg.motor_params.J = 0.00001f;
    cfg.motor_params.B = 0.01f;

    cfg.v_bus = 24.0f;
    return cfg;
}

TEST(ClosedLoop, PositionStepMovesTowardTarget) {
    SimAxisConfig cfg = make_sim_axis_cfg();
    SimAxisState st{};
    st.motor_state = PmsmState{0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    st.axis_state.pos.pos_pi = PI{2.0f, 0.0f, 0.0f, -200.0f, 200.0f};
    st.axis_state.spd.iq_pi = PI{1.0f, 0.0f, 0.0f, -200.0f, 200.0f};
    st.axis_state.foc.loop.id = PI{1.0f, 0.0f, 0.0f, -200.0f, 200.0f};
    st.axis_state.foc.loop.iq = PI{1.0f, 0.0f, 0.0f, -200.0f, 200.0f};

    float dt = 0.0005f;
    float theta_target = 1.0f;

    for (int k = 0; k < 20000; ++k) {
        sim_axis_step(st, cfg, dt, AxisMode::Position,
                      theta_target, 0.0f, 0.0f);
    }

    float theta_m = wrap_pi(st.motor_state.theta_e / cfg.motor_params.p);
    float err0 = theta_target - 0.0f;
    float err_final = theta_target - theta_m;

    EXPECT_LT(std::fabs(err_final), std::fabs(err0));
}