#pragma once

struct PmsmParams {
    float Rs;
    float Ls;
    float psi_m;
    float p;
    float J;
    float B;
};

struct PmsmState {
    float ia;
    float ib;
    float ic;
    float omega_m;
    float theta_e;
};

struct PmsmInput {
    float va;
    float vb;
    float vc;
    float T_L;
};

struct PmsmOutput {
    float ia;
    float ib;
    float ic;
    float omega_m;
    float theta_e;
    float torque;
};

PmsmOutput pmsm_step(
    PmsmState& state,
    const PmsmParams& params,
    const PmsmInput& input,
    float dt) noexcept;