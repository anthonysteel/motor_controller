#pragma once

#include "foc_math.hpp"

struct ModulationInput {
    AlphaBeta v_ab;
    float v_bus;
};

struct ModulationOutput {
    float m_a;
    float m_b;
    float m_c;
};

ModulationOutput run_modulation(const ModulationInput& in) noexcept;