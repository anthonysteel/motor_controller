#include <gtest/gtest.h>
#include "pi.hpp"

TEST(PiController, ZeroGainsAlwaysZero) {
    PI pi{0.0f, 0.0f, 0.0f, -10.0f, 10.0f};
    for (int k = 0; k < 10; ++k) {
        float u = pi.update(5.0f, 0.001f);
        EXPECT_FLOAT_EQ(u, 0.0f);
        EXPECT_FLOAT_EQ(pi.integral, 0.0f);
    }
}

TEST(PiController, PureProportional) {
    PI pi{2.0f, 0.0f, 0.0f, -100.0f, 100.0f};
    float u1 = pi.update(1.0f, 0.001f);
    float u2 = pi.update(-3.0f, 0.001f);
    EXPECT_FLOAT_EQ(u1, 2.0f);
    EXPECT_FLOAT_EQ(u2, -6.0f);
    EXPECT_FLOAT_EQ(pi.integral, 0.0f);
}

TEST(PiController, PureIntegralAccumulation) {
    PI pi{0.0f, 10.0f, 0.0f, -100.0f, 100.0f};
    float err = 1.0f;
    float dt = 0.001f;
    int steps = 1000;
    for (int k = 0; k < steps; ++k) {
        pi.update(err, dt);
    }
    float expected = 10.0f * err * dt * steps;
    EXPECT_NEAR(pi.integral, expected, 1e-3f);
    EXPECT_NEAR(pi.update(0.0f, dt), expected, 1e-3f);
}

TEST(PiController, OutputClampsToMax) {
    PI pi{50.0f, 0.0f, 0.0f, -10.0f, 10.0f};
    float u = pi.update(1.0f, 0.001f);
    EXPECT_FLOAT_EQ(u, 10.0f);
    EXPECT_FLOAT_EQ(pi.integral, 0.0f);
}

TEST(PiController, OutputClampsToMin) {
    PI pi{50.0f, 0.0f, 0.0f, -10.0f, 10.0f};
    float u = pi.update(-1.0f, 0.001f);
    EXPECT_FLOAT_EQ(u, -10.0f);
    EXPECT_FLOAT_EQ(pi.integral, 0.0f);
}

TEST(PiController, IntegralClampsWithinBounds) {
    PI pi{0.0f, 1000.0f, 0.0f, -5.0f, 5.0f};
    float err = 1.0f;
    float dt = 0.001f;
    for (int k = 0; k < 1000; ++k) {
        pi.update(err, dt);
    }
    EXPECT_LE(pi.integral, 5.0f + 1e-5f);
    EXPECT_GE(pi.integral, -5.0f - 1e-5f);
}

TEST(PiController, SaturatedOutputDoesNotGrowUnbounded) {
    PI pi{10.0f, 1000.0f, 0.0f, -5.0f, 5.0f};
    float err = 1.0f;
    float dt = 0.001f;
    float last_u = 0.0f;
    for (int k = 0; k < 1000; ++k) {
        float u = pi.update(err, dt);
        EXPECT_LE(u, 5.0f + 1e-5f);
        EXPECT_GE(u, -5.0f - 1e-5f);
        last_u = u;
    }
    EXPECT_NEAR(last_u, 5.0f, 1e-3f);
}

TEST(PiController, ResetSetsIntegral) {
    PI pi{1.0f, 10.0f, 0.0f, -10.0f, 10.0f};
    pi.update(1.0f, 0.1f);
    EXPECT_NE(pi.integral, 0.0f);
    pi.reset(3.0f);
    EXPECT_FLOAT_EQ(pi.integral, 3.0f);
    float u = pi.update(0.0f, 0.1f);
    EXPECT_FLOAT_EQ(u, 3.0f);
}
