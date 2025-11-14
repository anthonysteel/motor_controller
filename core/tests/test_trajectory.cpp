#include <gtest/gtest.h>
#include "trajectory.hpp"

TEST(Trajectory, ZeroConfigKeepsStateConstant) {
    TrajConfig cfg{0.0f, 0.0f};
    TrajState st{1.0f, 0.5f};
    TrajInput in{2.0f};

    auto out = run_traj_step(st, cfg, in, 0.001f);
    EXPECT_FLOAT_EQ(out.pos_ref, 1.0f);
    EXPECT_FLOAT_EQ(out.vel_ref, 0.5f);
}

TEST(Trajectory, MovesTowardTarget) {
    TrajConfig cfg{1.0f, 2.0f};
    TrajState st{0.0f, 0.0f};
    TrajInput in{1.0f};

    float dt = 0.001f;
    for (int k = 0; k < 2000; ++k) {
        auto out = run_traj_step(st, cfg, in, dt);
        EXPECT_GE(out.pos_ref, 0.0f);
        EXPECT_LE(out.pos_ref, in.target_pos + 1e-3f);
    }
    EXPECT_NEAR(st.pos, in.target_pos, 5e-2f);
}

TEST(Trajectory, RespectsVelocityLimit) {
    TrajConfig cfg{0.5f, 10.0f};
    TrajState st{0.0f, 0.0f};
    TrajInput in{10.0f};

    float dt = 0.001f;
    for (int k = 0; k < 5000; ++k) {
        auto out = run_traj_step(st, cfg, in, dt);
        EXPECT_LE(out.vel_ref, cfg.max_vel + 1e-4f);
        EXPECT_GE(out.vel_ref, -cfg.max_vel - 1e-4f);
    }
}

TEST(Trajectory, EventuallyStopsNearTarget) {
    TrajConfig cfg{1.0f, 2.0f};
    TrajState st{0.0f, 0.0f};
    TrajInput in{2.0f};

    float dt = 0.001f;
    for (int k = 0; k < 10000; ++k) {
        run_traj_step(st, cfg, in, dt);
    }
    EXPECT_NEAR(st.pos, in.target_pos, 5e-2f);
    EXPECT_NEAR(st.vel, 0.0f, 1e-2f);
}
