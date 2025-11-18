#pragma once

#include <cmath>

constexpr float pi_v        = 3.14159265358979323846f;
constexpr float two_pi_v    = 2.0f * pi_v;
constexpr float sqrt3_v     = 1.7320508075688772f;
constexpr float inv_sqrt3_v = 1.0f / sqrt3_v;
constexpr float two_thirds_v = 2.0f / 3.0f;

[[nodiscard]] inline float wrap_2pi(float angle) noexcept {
    float k = std::floor(angle / two_pi_v);
    angle -= k * two_pi_v;
    if (angle < 0.0f) {
        angle += two_pi_v;
    }
    return angle;
}

[[nodiscard]] inline float wrap_pi(float angle) noexcept {
    angle = wrap_2pi(angle);
    if (angle >= pi_v) {
        angle -= two_pi_v;
    }
    return angle;
}
