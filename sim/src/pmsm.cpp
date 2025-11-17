#include "pmsm.hpp"
#include "foc_math.hpp"
#include <cmath>

struct PmsmDeriv {
    float dia;
    float dib;
    float dic;
    float domega_m;
    float dtheta_e;
};

static PmsmDeriv pmsm_rhs(
    const PmsmState& x,
    const PmsmParams& params,
    const PmsmInput& input) noexcept
{
    PmsmDeriv dx{};

    float Rs = params.Rs;
    float Ls = params.Ls;
    float psi_m = params.psi_m;
    float p = params.p;
    float J = params.J;
    float B = params.B;

    float ia = x.ia;
    float ib = x.ib;
    float ic = x.ic;
    float omega_m = x.omega_m;
    float theta_e = x.theta_e;

    float va = input.va;
    float vb = input.vb;
    float vc = input.vc;
    float T_L = input.T_L;

    float omega_e = p * omega_m;

    dx.dia = (va - Rs * ia + psi_m * omega_e * std::sin(theta_e)) / Ls;
    dx.dib = (vb - Rs * ib + psi_m * omega_e * std::sin(theta_e - 2.0f * pi_v / 3.0f)) / Ls;
    dx.dic = (vc - Rs * ic + psi_m * omega_e * std::sin(theta_e + 2.0f * pi_v / 3.0f)) / Ls;

    float ialpha = (2.0f / 3.0f) * (ia - 0.5f * ib - 0.5f * ic);
    float ibeta = (2.0f / 3.0f) * ((sqrt3_v * 0.5f) * (ib - ic));

    float psia = Ls * ialpha + psi_m * std::cos(theta_e);
    float psib = Ls * ibeta + psi_m * std::sin(theta_e);

    float Te = 1.5f * p * (psia * ibeta - psib * ialpha);

    dx.domega_m = (Te - T_L - B * omega_m) / J;
    dx.dtheta_e = omega_e;

    return dx;
}

static PmsmState pmsm_add(
    const PmsmState& x,
    const PmsmDeriv& k,
    float step) noexcept
{
    PmsmState r{};
    r.ia = x.ia + step * k.dia;
    r.ib = x.ib + step * k.dib;
    r.ic = x.ic + step * k.dic;
    r.omega_m  = x.omega_m + step * k.domega_m;
    r.theta_e = x.theta_e + step * k.dtheta_e;
    return r;
}

PmsmOutput pmsm_step(
    PmsmState& state,
    const PmsmParams& params,
    const PmsmInput& input,
    float dt) noexcept
{
    PmsmOutput out{};

    if (dt <= 0.0f) {
        out.ia = state.ia;
        out.ib = state.ib;
        out.ic = state.ic;
        out.omega_m = state.omega_m;
        out.theta_e = state.theta_e;
        out.torque = 0.0f;
        return out;
    }

    PmsmState x = state;
    PmsmDeriv k1 = pmsm_rhs(x, params, input);
    PmsmDeriv k2 = pmsm_rhs(pmsm_add(x, k1, 0.5 * dt), params, input);
    PmsmDeriv k3 = pmsm_rhs(pmsm_add(x, k2, 0.5 * dt), params, input);
    PmsmDeriv k4 = pmsm_rhs(pmsm_add(x, k3, dt), params, input);

    constexpr float inv6 = 1.0f / 6.0f;
    PmsmDeriv k{};
    k.dia = (k1.dia + 2.0f * k2.dia + 2.0f * k3.dia + k4.dia) * inv6;
    k.dib = (k1.dib + 2.0f * k2.dib + 2.0f * k3.dib + k4.dib) * inv6;
    k.dic = (k1.dic + 2.0f * k2.dic + 2.0f * k3.dic + k4.dic) * inv6;
    k.domega_m = (k1.domega_m + 2.0f * k2.domega_m + 2.0f * k3.domega_m + k4.domega_m) * inv6;
    k.dtheta_e = (k1.dtheta_e + 2.0f * k2.dtheta_e + 2.0f * k3.dtheta_e + k4.dtheta_e) * inv6;

    x = pmsm_add(x, k, dt);
    x.theta_e = wrap_2pi(x.theta_e);

    state = x;

    float ia = x.ia;
    float ib = x.ib;
    float ic = x.ic;
    float omega_m = x.omega_m;
    float theta_e = x.theta_e;

    float Ls = params.Ls;
    float psi_m = params.psi_m;
    float p = params.p;

    float ialpha = (2.0f / 3.0f) * (ia - 0.5f * ib - 0.5f * ic);
    float ibeta = (2.0f / 3.0f) * ((sqrt3_v * 0.5f) * (ib - ic));

    float psia = Ls * ialpha + psi_m * std::cos(theta_e);
    float psib = Ls * ibeta + psi_m * std::sin(theta_e);
    
    float Te = 1.5 * p * (psia * ibeta - psib * ialpha);

    out.ia = ia;
    out.ib = ib;
    out.ic = ic;
    out.omega_m = omega_m;
    out.theta_e = theta_e;
    out.torque = Te;
    return out;
}
