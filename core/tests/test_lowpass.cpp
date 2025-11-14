#include <gtest/gtest.h>
#include "lowpass.hpp"

TEST(LowPass, AlphaZeroKeepsInitialValue) {
    LowPassConfig cfg{0.0f};
    LowPassState st{0.0f, false};

    float y0 = lowpass_update(st, cfg, 5.0f);
    EXPECT_FLOAT_EQ(y0, 5.0f);

    for (int i = 0; i < 10; ++i) {
        float y = lowpass_update(st, cfg, 10.0f);
        EXPECT_FLOAT_EQ(y, 5.0f);
    }
}

TEST(LowPass, AlphaOneTracksInput) {
    LowPassConfig cfg{1.0f};
    LowPassState st{0.0f, false};

    float y0 = lowpass_update(st, cfg, 2.0f);
    EXPECT_FLOAT_EQ(y0, 2.0f);

    float y1 = lowpass_update(st, cfg, 4.0f);
    EXPECT_FLOAT_EQ(y1, 4.0f);
}

TEST(LowPass, ConvergesToConstantInput) {
    LowPassConfig cfg{0.1f};
    LowPassState st{0.0f, false};

    float x = 10.0f;
    float y = 0.0f;

    for (int i = 0; i < 100; ++i) {
        y = lowpass_update(st, cfg, x);
    }

    EXPECT_NEAR(y, x, 1e-2f);
}
