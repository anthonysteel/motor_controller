#include "pi.hpp"

float PI::update(float error, float dt) noexcept {
    float i = integral + ki * error * dt;
    if (i > out_max) i = out_max;
    if (i < out_min) i = out_min;
    integral = i;

    float u = kp * error + integral;
    if (u > out_max) u = out_max;
    if (u < out_min) u = out_min;
    return u;
}

void PI::reset(float integral_init) noexcept {
    integral = integral_init;
}
