#include <gtest/gtest.h>
#include "foc_math.hpp"
#include "current_loop.hpp"

TEST(CurrentLoop, ZeroBusGivesZeroOutput) {
    CurrentLoopConfig cfg{0.8f};
    CurrentLoopState st{};
    st.id = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    CurrentLoopInput in{};
    in.i_meas = {0.0f, 0.0f};
    in.i_setpoint = {1.0f, 2.0f};
    in.v_bus = 0.0f;

    auto out = run_current_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.v_dq.d, 0.0f);
    EXPECT_FLOAT_EQ(out.v_dq.q, 0.0f);
}

TEST(CurrentLoop, ZeroModRadiusGivesZeroOutput) {
    CurrentLoopConfig cfg{0.0f};
    CurrentLoopState st{};
    st.id = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    CurrentLoopInput in{};
    in.i_meas = {0.0f, 0.0f};
    in.i_setpoint = {1.0f, 2.0f};
    in.v_bus = 24.0f;

    auto out = run_current_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.v_dq.d, 0.0f);
    EXPECT_FLOAT_EQ(out.v_dq.q, 0.0f);
}

TEST(CurrentLoop, ZeroErrorGivesZeroOutput) {
    CurrentLoopConfig cfg{0.8f};
    CurrentLoopState st{};
    st.id = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.iq = PI{1.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    CurrentLoopInput in{};
    in.i_meas = {1.0f, -2.0f};
    in.i_setpoint = {1.0f, -2.0f};
    in.v_bus = 24.0f;

    auto out = run_current_loop(st, cfg, in, 0.001f);
    EXPECT_NEAR(out.v_dq.d, 0.0f, 1e-6f);
    EXPECT_NEAR(out.v_dq.q, 0.0f, 1e-6f);
}

TEST(CurrentLoop, SimpleProportionalIqResponse) {
    CurrentLoopConfig cfg{1.0f};
    CurrentLoopState st{};
    st.id = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    st.iq = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    CurrentLoopInput in{};
    in.i_meas = {0.0f, 0.0f};
    in.i_setpoint = {0.0f, 3.0f};
    in.v_bus = 24.0f;

    auto out = run_current_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.v_dq.d, 0.0f);
    EXPECT_FLOAT_EQ(out.v_dq.q, 6.0f);
}

TEST(CurrentLoop, SaturatesToModRadiusTimesBus) {
    CurrentLoopConfig cfg{0.5f};
    CurrentLoopState st{};
    st.id = PI{0.0f, 0.0f, 0.0f, -1000.0f, 1000.0f};
    st.iq = PI{20.0f, 0.0f, 0.0f, -1000.0f, 1000.0f};

    CurrentLoopInput in{};
    in.i_meas = {0.0f, 0.0f};
    in.i_setpoint = {0.0f, 1.0f};
    in.v_bus = 10.0f;

    auto out = run_current_loop(st, cfg, in, 0.001f);
    float v_limit = cfg.mod_radius * in.v_bus;
    float mag = magnitude(out.v_dq);
    EXPECT_NEAR(mag, v_limit, 1e-3f);
    EXPECT_GT(out.v_dq.q, 0.0f);
}
