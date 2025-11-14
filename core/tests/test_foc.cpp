#include <gtest/gtest.h>
#include "foc.hpp"

TEST(Foc, ZeroCurrentsAndZeroSetpointGivesZeroOutput) {
    FocConfig cfg{};
    cfg.loop.mod_radius = 0.8f;

    FocState st{};
    st.loop.id = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.loop.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    FocInput in{};
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_elec = 0.0f;
    in.i_setpoint = {0.0f, 0.0f};
    in.v_bus = 24.0f;

    auto out = run_foc(st, cfg, in, 0.001f);

    EXPECT_NEAR(out.i_dq.d, 0.0f, 1e-6f);
    EXPECT_NEAR(out.i_dq.q, 0.0f, 1e-6f);
    EXPECT_NEAR(out.v_ab.alpha, 0.0f, 1e-6f);
    EXPECT_NEAR(out.v_ab.beta, 0.0f, 1e-6f);
}

TEST(Foc, PureIqSetpointAtZeroAngleProducesBetaVoltageOnly) {
    FocConfig cfg{};
    cfg.loop.mod_radius = 1.0f;

    FocState st{};
    st.loop.id = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.loop.iq = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    FocInput in{};
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_elec = 0.0f;
    in.i_setpoint = {0.0f, 3.0f};
    in.v_bus = 24.0f;

    auto out = run_foc(st, cfg, in, 0.001f);

    EXPECT_NEAR(out.v_ab.alpha, 0.0f, 1e-6f);
    EXPECT_NEAR(out.v_ab.beta, 6.0f, 1e-6f);
}

TEST(Foc, SaturatesToModRadiusTimesBusInQAxis) {
    FocConfig cfg{};
    cfg.loop.mod_radius = 0.5f;

    FocState st{};
    st.loop.id = PI{0.0f, 0.0f, 0.0f, -1000.0f, 1000.0f};
    st.loop.iq = PI{20.0f, 0.0f, 0.0f, -1000.0f, 1000.0f};

    FocInput in{};
    in.i_abc = {0.0f, 0.0f, 0.0f};
    in.theta_elec = 0.0f;
    in.i_setpoint = {0.0f, 1.0f};
    in.v_bus = 10.0f;

    auto out = run_foc(st, cfg, in, 0.001f);

    float v_limit = cfg.loop.mod_radius * in.v_bus;
    float mag = magnitude(out.v_ab);
    EXPECT_NEAR(mag, v_limit, 1e-3f);
    EXPECT_GT(out.v_ab.beta, 0.0f);
}

TEST(Foc, ZeroBusVoltageYieldsZeroVoltageButValidCurrents) {
    FocConfig cfg{};
    cfg.loop.mod_radius = 0.8f;

    FocState st{};
    st.loop.id = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.loop.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    FocInput in{};
    in.i_abc = {1.0f, -0.5f, -0.5f};
    in.theta_elec = 0.3f;
    in.i_setpoint = {0.0f, 0.0f};
    in.v_bus = 0.0f;

    auto out = run_foc(st, cfg, in, 0.001f);

    EXPECT_NEAR(out.v_ab.alpha, 0.0f, 1e-6f);
    EXPECT_NEAR(out.v_ab.beta, 0.0f, 1e-6f);
}
