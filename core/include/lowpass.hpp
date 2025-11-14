#pragma once

struct LowPassConfig {
    float alpha;
};

struct LowPassState {
    float y;
    bool initialized;
};

float lowpass_update(
    LowPassState& state,
    const LowPassConfig& cfg,
    float x) noexcept;