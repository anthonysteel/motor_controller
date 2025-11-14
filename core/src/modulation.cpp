#include "modulation.hpp"

ModulationOutput run_modulation(const ModulationInput& in) noexcept
{
    ModulationOutput out{};

    if (in.v_bus <= 0.0f) {
        out.m_a = 0.0f;
        out.m_b = 0.0f;
        out.m_c = 0.0f;
        return out;
    }

    PhaseCurrents v_abc = inv_clarke(in.v_ab);

    float half_vbus = 0.5 * in.v_bus;
    if (half_vbus <= 0.0f) {
        out.m_a = 0.0f;
        out.m_b = 0.0f;
        out.m_c = 0.0f;
        return out;
    }

    float m_a = v_abc.a / half_vbus;
    float m_b = v_abc.b / half_vbus;
    float m_c = v_abc.c / half_vbus;

    out.m_a = clamp(m_a, -1.0f, 1.0f);
    out.m_b = clamp(m_b, -1.0f, 1.0f);
    out.m_c = clamp(m_c, -1.0f, 1.0f);

    return out;
}