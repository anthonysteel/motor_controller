#include <gtest/gtest.h>
#include "foc_math.hpp"
#include "modulation.hpp"

TEST(Modulation, ZeroBusGivesZeroModulation) {
    ModulationInput in{};
    in.v_ab = {0.0f, 0.0f};
    in.v_bus = 0.0f;

    ModulationOutput out = run_modulation(in);
    EXPECT_FLOAT_EQ(out.m_a, 0.0f);
    EXPECT_FLOAT_EQ(out.m_b, 0.0f);
    EXPECT_FLOAT_EQ(out.m_c, 0.0f);
}

TEST(Modulation, ZeroVectorGivesZeroModulation) {
    ModulationInput in{};
    in.v_ab = {0.0f, 0.0f};
    in.v_bus = 24.0f;

    ModulationOutput out = run_modulation(in);
    EXPECT_FLOAT_EQ(out.m_a, 0.0f);
    EXPECT_FLOAT_EQ(out.m_b, 0.0f);
    EXPECT_FLOAT_EQ(out.m_c, 0.0f);
}

TEST(Modulation, SimpleAlphaVectorScalesCorrectly) {
    float v_bus = 20.0f;
    ModulationInput in{};
    in.v_ab = {0.25f * v_bus, 0.0f};
    in.v_bus = v_bus;

    ModulationOutput out = run_modulation(in);

    EXPECT_NEAR(out.m_a, 0.5f, 1e-6f);
    EXPECT_NEAR(out.m_b, -0.25f, 1e-6f);
    EXPECT_NEAR(out.m_c, -0.25f, 1e-6f);
}

TEST(Modulation, MagnitudeClampedToOne) {
    float v_bus = 10.0f;
    ModulationInput in{};
    in.v_ab = {10.0f * v_bus, 0.0f};
    in.v_bus = v_bus;

    ModulationOutput out = run_modulation(in);

    EXPECT_LE(out.m_a, 1.0f + 1e-6f);
    EXPECT_GE(out.m_a, -1.0f - 1e-6f);
    EXPECT_LE(out.m_b, 1.0f + 1e-6f);
    EXPECT_GE(out.m_b, -1.0f - 1e-6f);
    EXPECT_LE(out.m_c, 1.0f + 1e-6f);
    EXPECT_GE(out.m_c, -1.0f - 1e-6f);
}
