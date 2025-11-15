#include "modulation.hpp"

ModulationOutput run_modulation(const ModulationInput& in) noexcept
{
    ModulationOutput out{};
    out.m_a = 0.0f;
    out.m_b = 0.0f;
    out.m_c = 0.0f;
    out.saturated = false;

    if (in.v_bus <= 0.0f) {
        return out;
    }

    float half_vbus = 0.5 * in.v_bus;
    if (half_vbus <= 0.0f) {
        return out;
    }

    float v_alpha = in.v_ab.alpha;
    float v_beta = in.v_ab.beta;

    PhaseCurrents v_abc = inv_clarke(AlphaBeta{v_alpha, v_beta});

    float x_a = v_abc.a / half_vbus;
    float x_b = v_abc.b / half_vbus;
    float x_c = v_abc.c / half_vbus;

    float max_x = x_a;
    if (x_b > max_x) max_x = x_b;
    if (x_c > max_x) max_x = x_c;

    float min_x = x_a;
    if (x_b < min_x) min_x = x_b;
    if (x_c < min_x) min_x = x_c;

    float z = -0.5f * (max_x + min_x);

    float m_a = x_a + z;
    float m_b = x_b + z;
    float m_c = x_c + z;

    float max_abs = std::fabs(m_a);
    if (std::fabs(m_b) > max_abs) max_abs = std::fabs(m_b);
    if (std::fabs(m_c) > max_abs) max_abs = std::fabs(m_c);

    if (max_abs > 1.0f && max_abs > 0.0f) {
        float s = 1.0f / max_abs;
        m_a *= s;
        m_b *= s;
        m_c *= s;
        out.saturated = true;
    }

    out.m_a = clamp(m_a, -1.0f, 1.0f);
    out.m_b = clamp(m_b, -1.0f, 1.0f);
    out.m_c = clamp(m_c, -1.0f, 1.0f);

    return out;
}