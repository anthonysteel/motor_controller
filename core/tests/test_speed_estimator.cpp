#include <gtest/gtest.h>
#include "speed_estimator.hpp"
#include "foc_math.hpp"

TEST(SpeedEstimator, InitializesToZero) {
    SpeedEstimatorConfig cfg{{0.5f}};
    SpeedEstimatorState st{};
    st.initialized = false;
    st.lp.initialized = false;

    SpeedEstimatorInput in{};
    in.theta_meas = 1.0f;

    SpeedEstimatorOutput out = run_speed_estimator(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.w_raw, 0.0f);
    EXPECT_FLOAT_EQ(out.w_filtered, 0.0f);
}

TEST(SpeedEstimator, ZeroDtReturnsPreviousValues) {
    SpeedEstimatorConfig cfg{{0.5f}};
    SpeedEstimatorState st{};
    st.initialized = true;
    st.theta_prev = 0.0f;
    st.w_raw = 3.0f;
    st.lp.y = 2.0f;
    st.lp.initialized = true;

    SpeedEstimatorInput in{};
    in.theta_meas = 0.5f;

    SpeedEstimatorOutput out = run_speed_estimator(st, cfg, in, 0.0f);
    EXPECT_FLOAT_EQ(out.w_raw, 3.0f);
    EXPECT_FLOAT_EQ(out.w_filtered, 2.0f);
}

TEST(SpeedEstimator, TracksConstantVelocity) {
    SpeedEstimatorConfig cfg{{0.2f}};
    SpeedEstimatorState st{};
    st.initialized = false;
    st.lp.initialized = false;

    float dt = 0.001f;
    float w_true = 50.0f;
    float theta = 0.0f;

    for (int i = 0; i < 5000; ++i) {
        theta = wrap_2pi(theta + w_true * dt);
        SpeedEstimatorInput in{theta};
        SpeedEstimatorOutput out = run_speed_estimator(st, cfg, in, dt);
        if (i > 1000) {
            EXPECT_NEAR(out.w_filtered, w_true, 5.0f);
        }
    }

    EXPECT_NEAR(st.w_raw, w_true, 1.0f);
}
