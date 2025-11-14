#pragma once

#include <cmath>
#include <type_traits>

struct PhaseCurrents {
    float a, b, c;
};

struct AlphaBeta {
    float alpha, beta;
};

struct DQ {
    float d, q;
};

constexpr float pi_v          = 3.14159265358979323846f;
constexpr float two_pi_v      = 2.0f * pi_v;
constexpr float sqrt3_v       = 1.7320508075688772f;
constexpr float inv_sqrt3_v   = 1.0f / sqrt3_v;
constexpr float two_thirds_v  = 0.66666668653488159f;

template <typename T>
[[nodiscard]] inline T clamp(T x, T lo, T hi) noexcept {
    return x < lo ? lo : (x > hi ? hi : x);
}

[[nodiscard]] inline float signf(float x) noexcept {
    return x > 0.0f ? 1.0f : (x < 0.0f ? -1.0f : 0.0f);
}


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

[[nodiscard]] inline AlphaBeta clarke(const PhaseCurrents& i) noexcept {
    AlphaBeta ab;
    ab.alpha = i.a;
    ab.beta  = (i.a + 2.0f * i.b) * inv_sqrt3_v;
    return ab;
}

[[nodiscard]] inline PhaseCurrents inv_clarke(const AlphaBeta& ab) noexcept {
    PhaseCurrents i;
    i.a = ab.alpha;
    i.b = (-ab.alpha + sqrt3_v * ab.beta) * 0.5f;
    i.c = (-ab.alpha - sqrt3_v * ab.beta) * 0.5f;
    return i;
}

[[nodiscard]] inline DQ park(const AlphaBeta& ab, float theta) noexcept {
    float s = std::sin(theta);
    float c = std::cos(theta);
    DQ dq;
    dq.d =  c * ab.alpha + s * ab.beta;
    dq.q = -s * ab.alpha + c * ab.beta;
    return dq;
}

[[nodiscard]] inline AlphaBeta inv_park(const DQ& dq, float theta) noexcept {
    float s = std::sin(theta);
    float c = std::cos(theta);
    AlphaBeta ab;
    ab.alpha =  c * dq.d - s * dq.q;
    ab.beta  =  s * dq.d + c * dq.q;
    return ab;
}

[[nodiscard]] inline float magnitude(const AlphaBeta& v) noexcept {
    return std::sqrt(v.alpha * v.alpha + v.beta * v.beta);
}

[[nodiscard]] inline float magnitude(const DQ& v) noexcept {
    return std::sqrt(v.d * v.d + v.q * v.q);
}

template <typename Vec>
inline void saturate(Vec& v, float max_mag) noexcept {
    static_assert(std::is_same_v<Vec, AlphaBeta> || std::is_same_v<Vec, DQ>);
    float m = magnitude(v);
    if (m > max_mag && m > 0.0f) {
        float scale = max_mag / m;
        if constexpr (std::is_same_v<Vec, AlphaBeta>) {
            v.alpha *= scale;
            v.beta  *= scale;
        } else {
            v.d *= scale;
            v.q *= scale;
        }
    }
}
