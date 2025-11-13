#include <gtest/gtest.h>
#include "foc_math.hpp"

TEST(FocMathCompileTest, HeaderCompiles) {
    SUCCEED();
}

TEST(FocMathClamp, BasicCases) {
    EXPECT_EQ(clamp(5.0f, 0.0f, 10.0f), 5.0f);
    EXPECT_EQ(clamp(-1.0f, 0.0f, 10.0f), 0.0f);
    EXPECT_EQ(clamp(11.0f, 0.0f, 10.0f), 10.0f);
}

TEST(FocMathWrap2Pi, MultiplesOfTwoPi) {
    EXPECT_NEAR(wrap_2pi(0.0f), 0.0f, 1e-6f);
    EXPECT_NEAR(wrap_2pi(two_pi_v), 0.0f, 1e-6f);
    EXPECT_NEAR(wrap_2pi(-two_pi_v), 0.0f, 1e-6f);
}

TEST(FocMathWrap2Pi, LargeAngles) {
    float a = 10.0f * two_pi_v + 0.123f;
    float w = wrap_2pi(a);
    EXPECT_GE(w, 0.0f);
    EXPECT_LT(w, two_pi_v);
    EXPECT_NEAR(w, 0.123f, 1e-5f);

    float b = -7.0f * two_pi_v + 0.5f;
    float w2 = wrap_2pi(b);
    EXPECT_GE(w2, 0.0f);
    EXPECT_LT(w2, two_pi_v);
    EXPECT_NEAR(w2, 0.5f, 1e-5f);
}

TEST(FocMathWrapPi, AroundPi) {
    EXPECT_NEAR(wrap_pi(0.0f), 0.0f, 1e-6f);
    EXPECT_NEAR(wrap_pi(pi_v), -pi_v, 1e-6f);
    EXPECT_NEAR(wrap_pi(-pi_v), -pi_v, 1e-6f);
    EXPECT_NEAR(wrap_pi(pi_v - 0.1f), pi_v - 0.1f, 1e-6f);
    EXPECT_NEAR(wrap_pi(pi_v + 0.1f), -pi_v + 0.1f, 1e-6f);
}

TEST(FocMathRoundTrip, ClarkeInverseClarkeBalanced) {
    PhaseCurrents i{1.0f, -0.5f, -0.5f};
    AlphaBeta ab = clarke(i);
    PhaseCurrents i_back = inv_clarke(ab);

    EXPECT_NEAR(i_back.a, i.a, 1e-5f);
    EXPECT_NEAR(i_back.b, i.b, 1e-5f);
    EXPECT_NEAR(i_back.c, i.c, 1e-5f);
}

TEST(FocMathRoundTrip, ClarkeInverseClarkeGeneral) {
    PhaseCurrents i{0.3f, -1.2f, 0.9f};
    AlphaBeta ab = clarke(i);
    PhaseCurrents i_back = inv_clarke(ab);

    EXPECT_NEAR(i_back.a, i.a, 1e-4f);
    EXPECT_NEAR(i_back.b, i.b, 1e-4f);
    EXPECT_NEAR(i_back.c, i.c, 1e-4f);
}

TEST(FocMathRoundTrip, ParkInversePark) {
    AlphaBeta ab{1.0f, -0.25f};
    float theta = 0.72f;

    DQ dq = park(ab, theta);
    AlphaBeta ab_back = inv_park(dq, theta);

    EXPECT_NEAR(ab_back.alpha, ab.alpha, 1e-5f);
    EXPECT_NEAR(ab_back.beta,  ab.beta,  1e-5f);
}

TEST(FocMathMagnitude, SimpleVectors) {
    AlphaBeta ab{3.0f, 4.0f};
    EXPECT_NEAR(magnitude(ab), 5.0f, 1e-6f);

    DQ dq{0.0f, 0.0f};
    EXPECT_NEAR(magnitude(dq), 0.0f, 1e-6f);
}

TEST(FocMathSaturate, NoSaturationWhenBelowLimit) {
    AlphaBeta v{0.6f, 0.0f};
    float max_mag = 1.0f;
    float before = magnitude(v);
    saturate(v, max_mag);
    EXPECT_NEAR(magnitude(v), before, 1e-6f);
}

TEST(FocMathSaturate, NoSaturationWhenAtLimit) {
    AlphaBeta v{1.0f, 0.0f};
    float max_mag = 1.0f;
    saturate(v, max_mag);
    EXPECT_NEAR(magnitude(v), 1.0f, 1e-6f);
}

TEST(FocMathSaturate, SaturatesWhenAboveLimitAlphaBeta) {
    AlphaBeta v{2.0f, 0.0f};
    float max_mag = 1.0f;
    saturate(v, max_mag);
    EXPECT_NEAR(magnitude(v), max_mag, 1e-6f);
    EXPECT_GT(v.alpha, 0.0f);
}

TEST(FocMathSaturate, SaturatesWhenAboveLimitDQ) {
    DQ v{0.0f, 2.0f};
    float max_mag = 1.0f;
    saturate(v, max_mag);
    EXPECT_NEAR(magnitude(v), max_mag, 1e-6f);
    EXPECT_GT(v.q, 0.0f);
}
