#include <cmath>
#include <gtest/gtest.h>
#include "pmsm.hpp"
#include "sim_math.hpp"

static PmsmParams make_default_params() {
    PmsmParams p{};
    p.Rs = 0.1f;
    p.Ls = 0.001f;
    p.psi_m = 0.05f;
    p.p = 4.0f;
    p.J = 0.0001f;
    p.B = 0.0001f;
    return p;
}

TEST(Pmsm, ZeroVoltagesHoldStateWithNoLoad) {
    PmsmParams params = make_default_params();
    PmsmState st{};
    st.ia = 0.0f;
    st.ib = 0.0f;
    st.ic = 0.0f;
    st.omega_m = 0.0f;
    st.theta_e = 0.0f;

    PmsmInput in{};
    in.va = 0.0f;
    in.vb = 0.0f;
    in.vc = 0.0f;
    in.T_L = 0.0f;

    float dt = 1e-5f;
    for (int k = 0; k < 2000; ++k) {
        pmsm_step(st, params, in, dt);
    }

    EXPECT_NEAR(st.ia, 0.0f, 1e-4f);
    EXPECT_NEAR(st.ib, 0.0f, 1e-4f);
    EXPECT_NEAR(st.ic, 0.0f, 1e-4f);
    EXPECT_NEAR(st.omega_m, 0.0f, 1e-4f);
}

TEST(Pmsm, SinusoidalBackEmfDriveProducesAcceleration) {
    PmsmParams params = make_default_params();
    PmsmState st{};
    st.ia = 0.0f;
    st.ib = 0.0f;
    st.ic = 0.0f;
    st.omega_m = 0.0f;
    st.theta_e = 0.0f;

    PmsmInput in{};
    in.T_L = 0.0f;

    float dt = 1e-5f;
    float v_amp = 5.0f;

    for (int k = 0; k < 10000; ++k) {
        float th = st.theta_e;
        in.va = v_amp * std::sin(th);
        in.vb = v_amp * std::sin(th + 2.0f * pi_v / 3.0f);
        in.vc = v_amp * std::sin(th - 2.0f * pi_v / 3.0f);
        pmsm_step(st, params, in, dt);
    }

    EXPECT_GT(std::fabs(st.omega_m), 1e-3f);
}
