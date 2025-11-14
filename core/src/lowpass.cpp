#include "lowpass.hpp"
#include "foc_math.hpp"

float lowpass_update(
    LowPassState& state,
    const LowPassConfig& cfg,
    float x) noexcept
{
    float a = clamp(cfg.alpha, 0.0f, 1.0f);

    if (!state.initialized) {
        state.y = x;
        state.initialized = true;
        return state.y;
    }

    state.y += a * (x - state.y);
    return state.y;
}