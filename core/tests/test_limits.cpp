#include <gtest/gtest.h>
#include "limits.hpp"

TEST(Limits, IqWithinBoundsUnchanged) {
    LimitsConfig cfg{-10.0f, 10.0f, -100.0f, 100.0f};
    LimitsState st{false, false};

    float iq = apply_iq_limit(st, cfg, 5.0f);
    EXPECT_FLOAT_EQ(iq, 5.0f);
    EXPECT_FALSE(st.iq_limited);
}

TEST(Limits, IqClampedToMax) {
    LimitsConfig cfg{-10.0f, 10.0f, -100.0f, 100.0f};
    LimitsState st{false, false};

    float iq = apply_iq_limit(st, cfg, 20.0f);
    EXPECT_FLOAT_EQ(iq, 10.0f);
    EXPECT_TRUE(st.iq_limited);
}

TEST(Limits, IqClampedToMin) {
    LimitsConfig cfg{-10.0f, 10.0f, -100.0f, 100.0f};
    LimitsState st{false, false};

    float iq = apply_iq_limit(st, cfg, -20.0f);
    EXPECT_FLOAT_EQ(iq, -10.0f);
    EXPECT_TRUE(st.iq_limited);
}

TEST(Limits, VelWithinBoundsUnchanged) {
    LimitsConfig cfg{-10.0f, 10.0f, -5.0f, 5.0f};
    LimitsState st{false, false};

    float w = apply_vel_limit(st, cfg, 2.0f);
    EXPECT_FLOAT_EQ(w, 2.0f);
    EXPECT_FALSE(st.w_limited);
}

TEST(Limits, VelClampedToMax) {
    LimitsConfig cfg{-10.0f, 10.0f, -5.0f, 5.0f};
    LimitsState st{false, false};

    float w = apply_vel_limit(st, cfg, 20.0f);
    EXPECT_FLOAT_EQ(w, 5.0f);
    EXPECT_TRUE(st.w_limited);
}

TEST(Limits, VelClampedToMin) {
    LimitsConfig cfg{-10.0f, 10.0f, -5.0f, 5.0f};
    LimitsState st{false, false};

    float w = apply_vel_limit(st, cfg, -20.0f);
    EXPECT_FLOAT_EQ(w, -5.0f);
    EXPECT_TRUE(st.w_limited);
}
