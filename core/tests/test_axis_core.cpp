#include <gtest/gtest.h>
#include "axis_core.hpp"

static AxisCoreConfig make_default_axis_cfg() {
    AxisCoreConfig cfg{};
    cfg.traj = TrajConfig{1.0f, 2.0f};
    cfg.pos  = PositionLoopConfig{-100.0f, 100.0f};
    cfg.spd  = SpeedLoopConfig{-100.0f, 100.0f};
    cfg.cur  = CurrentLoopConfig{0.8f};
    cfg.foc  = FocConfig{cfg.cur};
    cfg.est  = SpeedEstimatorConfig{LowPassConfig{0.2f}};
    cfg.lim  = LimitsConfig{-100.0f, 100.0f, -100.0f, 100.0f};
    return cfg;
}

TEST(AxisCore, IdleModeProducesZeroModulation) {
    AxisCoreConfig cfg = make_default_axis_cfg();
    AxisCoreState st{};

    AxisCoreInput in{};
    in.mode = AxisMode::Idle;
    in.theta_meas = 0.0f;
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_target = 0.0f;
    in.w_target = 0.0f;
    in.iq_target = 0.0f;
    in.v_bus = 24.0f;
    in.theta_elec = 0.0f;

    AxisCoreOutput out = run_axis_core(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.m_a, 0.0f);
    EXPECT_FLOAT_EQ(out.m_b, 0.0f);
    EXPECT_FLOAT_EQ(out.m_c, 0.0f);
}

TEST(AxisCore, CurrentIqModeUsesIqTarget) {
    AxisCoreConfig cfg = make_default_axis_cfg();
    AxisCoreState st{};
    st.foc.loop.id = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    AxisCoreInput in{};
    in.mode = AxisMode::CurrentIq;
    in.theta_meas = 0.0f;
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_target = 0.0f;
    in.w_target = 0.0f;
    in.iq_target = 3.0f;
    in.v_bus = 24.0f;
    in.theta_elec = 0.0f;

    AxisCoreOutput out = run_axis_core(st, cfg, in, 0.001f);
    EXPECT_NEAR(out.iq_cmd, 3.0f, 1e-4f);
}

TEST(AxisCore, VelocityModeProducesPositiveIqForPositiveSpeed) {
    AxisCoreConfig cfg = make_default_axis_cfg();
    AxisCoreState st{};
    st.spd.iq_pi = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.id = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    AxisCoreInput in{};
    in.mode = AxisMode::Velocity;
    in.theta_meas = 0.0f;
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_target = 0.0f;
    in.w_target = 5.0f;
    in.iq_target = 0.0f;
    in.v_bus = 24.0f;
    in.theta_elec = 0.0f;

    AxisCoreOutput out = run_axis_core(st, cfg, in, 0.001f);
    EXPECT_GT(out.iq_cmd, 0.0f);
}

TEST(AxisCore, PositionModeMovesThetaRefTowardTarget) {
    AxisCoreConfig cfg = make_default_axis_cfg();
    AxisCoreState st{};
    st.pos.pos_pi = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.spd.iq_pi = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.id = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    AxisCoreInput in{};
    in.mode = AxisMode::Position;
    in.theta_meas = 0.0f;
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_target = 1.0f;
    in.w_target = 0.0f;
    in.iq_target = 0.0f;
    in.v_bus = 24.0f;
    in.theta_elec = 0.0f;

    float dt = 0.001f;
    float last_theta_ref = 0.0f;
    for (int k = 0; k < 100; ++k) {
        AxisCoreOutput out = run_axis_core(st, cfg, in, dt);
        EXPECT_GE(out.theta_ref, last_theta_ref - 1e-6f);
        last_theta_ref = out.theta_ref;
    }
}

TEST(AxisCore, IqLimitFlagIsSetWhenClamped) {
    AxisCoreConfig cfg = make_default_axis_cfg();
    cfg.lim.iq_max = 1.0f;
    cfg.lim.iq_min = -1.0f;

    AxisCoreState st{};
    st.spd.iq_pi = PI{10.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.id = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.foc.loop.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    AxisCoreInput in{};
    in.mode = AxisMode::Velocity;
    in.theta_meas = 0.0f;
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_target = 0.0f;
    in.w_target = 10.0f;
    in.iq_target = 0.0f;
    in.v_bus = 24.0f;
    in.theta_elec = 0.0f;

    AxisCoreOutput out = run_axis_core(st, cfg, in, 0.001f);
    EXPECT_LE(out.iq_cmd, 1.0f + 1e-6f);
    EXPECT_TRUE(out.status.iq_limited);
}
