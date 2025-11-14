#include <gtest/gtest.h>
#include "foc_math.hpp"
#include "position_loop.hpp"

TEST(PositionLoop, ZeroGainsAlwaysZeroSpeed) {
    PositionLoopConfig cfg{-100.0f, 100.0f};
    PositionLoopState st{};
    st.pos_pi = PI{0.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    PositionLoopInput in{};
    in.theta_meas = 1.0f;
    in.theta_setpoint = 3.0f;

    for (int k = 0; k < 10; ++k) {
        PositionLoopOutput out = run_position_loop(st, cfg, in, 0.001f);
        EXPECT_FLOAT_EQ(out.w_cmd, 0.0f);
        EXPECT_FLOAT_EQ(st.pos_pi.integral, 0.0f);
    }
}

TEST(PositionLoop, PureProportionalResponse) {
    PositionLoopConfig cfg{-100.0f, 100.0f};
    PositionLoopState st{};
    st.pos_pi = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    PositionLoopInput in{};
    in.theta_meas = 1.0f;
    in.theta_setpoint = 3.0f;

    PositionLoopOutput out = run_position_loop(st, cfg, in, 0.001f);
    float err = wrap_pi(in.theta_setpoint - in.theta_meas);
    EXPECT_FLOAT_EQ(out.w_cmd, 2.0f * err);
    EXPECT_FLOAT_EQ(st.pos_pi.integral, 0.0f);
}

TEST(PositionLoop, IntegralAccumulation) {
    PositionLoopConfig cfg{-100.0f, 100.0f};
    PositionLoopState st{};
    st.pos_pi = PI{0.0f, 10.0f, 0.0f, -100.0f, 100.0f};

    PositionLoopInput in{};
    in.theta_meas = 0.0f;
    in.theta_setpoint = 1.0f;

    float dt = 0.001f;
    int steps = 1000;
    for (int k = 0; k < steps; ++k) {
        run_position_loop(st, cfg, in, dt);
    }

    float err = wrap_pi(in.theta_setpoint - in.theta_meas);
    float expected = 10.0f * err * dt * steps;
    EXPECT_NEAR(st.pos_pi.integral, expected, 5e-3f);
}

TEST(PositionLoop, OutputClampedToMax) {
    PositionLoopConfig cfg{-5.0f, 5.0f};
    PositionLoopState st{};
    st.pos_pi = PI{50.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    PositionLoopInput in{};
    in.theta_meas = 0.0f;
    in.theta_setpoint = 1.0f;

    PositionLoopOutput out = run_position_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.w_cmd, 5.0f);
}

TEST(PositionLoop, OutputClampedToMin) {
    PositionLoopConfig cfg{-5.0f, 5.0f};
    PositionLoopState st{};
    st.pos_pi = PI{50.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    PositionLoopInput in{};
    in.theta_meas = 1.0f;
    in.theta_setpoint = 0.0f;

    PositionLoopOutput out = run_position_loop(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.w_cmd, -5.0f);
}

TEST(PositionLoop, UsesShortestWrappedError) {
    PositionLoopConfig cfg{-100.0f, 100.0f};
    PositionLoopState st{};
    st.pos_pi = PI{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};

    PositionLoopInput in{};
    in.theta_meas = two_pi_v - 0.1f;
    in.theta_setpoint = 0.0f;

    PositionLoopOutput out = run_position_loop(st, cfg, in, 0.001f);
    float err = wrap_pi(in.theta_setpoint - in.theta_meas);
    EXPECT_NEAR(err, 0.1f, 1e-4f);
    EXPECT_NEAR(out.w_cmd, 2.0f * 0.1f, 1e-3f);
}
