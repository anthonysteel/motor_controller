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
    EXPECT_FALSE(out.saturated);
}

TEST(Modulation, ZeroVectorGivesZeroModulation) {
    ModulationInput in{};
    in.v_ab = {0.0f, 0.0f};
    in.v_bus = 24.0f;

    ModulationOutput out = run_modulation(in);
    EXPECT_FLOAT_EQ(out.m_a, 0.0f);
    EXPECT_FLOAT_EQ(out.m_b, 0.0f);
    EXPECT_FLOAT_EQ(out.m_c, 0.0f);
    EXPECT_FALSE(out.saturated);
}

TEST(Modulation, SmallVectorStaysUnsaturatedAndBounded) {
    float v_bus = 20.0f;
    ModulationInput in{};
    in.v_ab = {0.1f * v_bus, 0.0f};
    in.v_bus = v_bus;

    ModulationOutput out = run_modulation(in);

    EXPECT_GE(out.m_a, -1.0f);
    EXPECT_LE(out.m_a,  1.0f);
    EXPECT_GE(out.m_b, -1.0f);
    EXPECT_LE(out.m_b,  1.0f);
    EXPECT_GE(out.m_c, -1.0f);
    EXPECT_LE(out.m_c,  1.0f);
    EXPECT_FALSE(out.saturated);
}

TEST(Modulation, LargeVectorTriggersSaturationAndClamp) {
    float v_bus = 10.0f;
    ModulationInput in{};
    in.v_ab = {10.0f * v_bus, 0.0f};
    in.v_bus = v_bus;

    ModulationOutput out = run_modulation(in);

    float max_abs = std::fabs(out.m_a);
    if (std::fabs(out.m_b) > max_abs) max_abs = std::fabs(out.m_b);
    if (std::fabs(out.m_c) > max_abs) max_abs = std::fabs(out.m_c);

    EXPECT_NEAR(max_abs, 1.0f, 1e-5f);
    EXPECT_TRUE(out.saturated);
}