#include <gtest/gtest.h>
#include "speed_loop.hpp"

TEST(SpeedLoop, ZeroGainsAlwaysZeroIq) {
    SpeedLoopConfig cfg{-100.0f, 100.0f};
    SpeedLoopState st{};
    st.iq_pi = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    SpeedLoopInput in{};
    in.w_meas = 0.0f;
    in.w_setpoint = 10.0f;

    for (int k = 0; k < 10; ++k) {
        SpeedLoopOutput out = run_speed_loop(st, cfg, in, 0.001f);
        EXPECT_FLOAT_EQ(out.iq_cmd, 0.0f);
        EXPECT_FLOAT_EQ(st.iq_pi.integral, 0.0f);
    }
}

TEST(SpeedLoop, PureProportionalResponse) {
    SpeedLoopConfig cfg{-100.0f, 100.0f};
    SpeedLoopState st{};
    st.iq_pi = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    SpeedLoopInput in{};
    in.w_meas = 5.0f;
    in.w_setpoint = 8.0f;

    SpeedLoopOutput out = run_speed_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.iq_cmd, 6.0f);
    EXPECT_FLOAT_EQ(st.iq_pi.integral, 0.0f);
}

TEST(SpeedLoop, PureIntegralAccumulation) {
    SpeedLoopConfig cfg{-100.0f, 100.0f};
    SpeedLoopState st{};
    st.iq_pi = PI{0.0f, 10.0f, 0.0f, -100.0f, 100.0f};

    SpeedLoopInput in{};
    in.w_meas = 0.0f;
    in.w_setpoint = 1.0f;

    float dt = 0.001f;
    int steps = 1000;
    for (int k = 0; k < steps; ++k) {
        run_speed_loop(st, cfg, in, dt);
    }

    float expected = 10.0f * (in.w_setpoint - in.w_meas) * dt * steps;
    EXPECT_NEAR(st.iq_pi.integral, expected, 1e-3f);

    in.w_setpoint = in.w_meas;
    SpeedLoopOutput out = run_speed_loop(st, cfg, in, dt);
    EXPECT_NEAR(out.iq_cmd, expected, 1e-3f);
}


TEST(SpeedLoop, OutputClampedToMax) {
    SpeedLoopConfig cfg{-5.0f, 5.0f};
    SpeedLoopState st{};
    st.iq_pi = PI{50.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    SpeedLoopInput in{};
    in.w_meas = 0.0f;
    in.w_setpoint = 1.0f;

    SpeedLoopOutput out = run_speed_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.iq_cmd, 5.0f);
}

TEST(SpeedLoop, OutputClampedToMin) {
    SpeedLoopConfig cfg{-5.0f, 5.0f};
    SpeedLoopState st{};
    st.iq_pi = PI{50.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    SpeedLoopInput in{};
    in.w_meas = 1.0f;
    in.w_setpoint = 0.0f;

    SpeedLoopOutput out = run_speed_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.iq_cmd, -5.0f);
}
