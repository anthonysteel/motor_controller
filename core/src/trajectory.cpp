#include "trajectory.hpp"
#include "foc_math.hpp"

static inline float signf(float x) noexcept {
    return x > 0.0f ? 1.0f : (x < 0.0f ? -1.0f : 0.0f);
}

TrajOutput run_traj_step(
    TrajState& state,
    const TrajConfig& cfg,
    const TrajInput& in,
    float dt) noexcept
{
    TrajOutput out{};

    if (dt <= 0.0f || cfg.max_acc <= 0.0f || cfg.max_vel <= 0.0f) {
        out.pos_ref = state.pos;
        out.vel_ref = state.vel;
        return out;
    }

    float err = in.target_pos - state.pos;
    float s = signf(err);

    float v = state.vel;
    float a = 0.0f;

    if (s == 0.0f && std::fabs(v) < 1e-6f) {
        v = 0.0f;
        a = 0.0f;
    } else {
        float v_abs = std::fabs(v);
        float d_stop = 0.5f * v_abs * v_abs / cfg.max_acc;

        if (std::fabs(err) <= d_stop) {
            a = -signf(v) * cfg.max_acc;
        } else {
            a = s * cfg.max_acc;
        }
    }

    v += a * dt;
    v = clamp(v, -cfg.max_vel, cfg.max_vel);

    float p = state.pos + v * dt;

    if (std::fabs(in.target_pos - p) < 1e-6f && std::fabs(v) < 1e-4f) {
        p = in.target_pos;
        v = 0.0f;
    }

    state.pos = p;
    state.vel = v;

    out.pos_ref = p;
    out.vel_ref = v;
    return out;
}