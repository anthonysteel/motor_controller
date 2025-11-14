#pragma once

struct TrajConfig {
    float max_vel;
    float max_acc;
};

struct TrajState {
    float pos;
    float vel;
};

struct TrajInput {
    float target_pos;
};

struct TrajOutput {
    float pos_ref;
    float vel_ref;
};

TrajOutput run_traj_step(
    TrajState& state,
    const TrajConfig& cfg,
    const TrajInput& in,
    float dt) noexcept;