/*
    Virtual bass plugin for DeaDBeeF
    Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
    The MIT License (MIT)
*/
#include <math.h>

#include "virtual_bass.h"
#define M_PI_2 1.57079632679489661923

static biquad_s_coefs s_coefs[N_STAGES] = {
    {.A = 1.0, .B = 1.931851652578, .C = 1.0, .D = 0.0, .E = 0.0, .F = 1.0, },
    {.A = 1.0, .B = 1.414213562373, .C = 1.0, .D = 0.0, .E = 0.0, .F = 1.0, },
    {.A = 1.0, .B = 0.517638090205, .C = 1.0, .D = 0.0, .E = 0.0, .F = 1.0, }};

void bilinear_biquad_design_lpf(biquad_z_coefs *out, const biquad_s_coefs *in,
                                float omega) {
    float T = 2.0 * tan(omega * M_PI_2);
    float Arg = (4.0 * in->A + 2.0 * in->B * T + in->C * T * T);
    out->a2 = (4.0 * in->A - 2.0 * in->B * T + in->C * T * T) / Arg;
    out->a1 = (2.0 * in->C * T * T - 8.0 * in->A) / Arg;
    out->a0 = 1.0;

    out->b2 =
        (4.0 * in->D - 2.0 * in->E * T + in->F * T * T) / Arg * in->C / in->F;
    out->b1 = (2.0 * in->F * T * T - 8.0 * in->D) / Arg * in->C / in->F;
    out->b0 =
        (4 * in->D + in->F * T * T + 2.0 * in->E * T) / Arg * in->C / in->F;
}

void biquad_reset(biquad_buffer *buf) {
    buf->x1 = buf->x2 = buf->y1 = buf->y2 = 0;
}

float biquad_process(biquad_buffer *buf, const biquad_z_coefs *coefs, float x) {
    float y, center_tap;

    center_tap = x * coefs->b0 + coefs->b1 * buf->x1 + coefs->b2 * buf->x2;
    y = coefs->a0 * center_tap - coefs->a1 * buf->y1 - coefs->a2 * buf->y2;

    buf->x2 = buf->x1;
    buf->x1 = x;
    buf->y2 = buf->y1;
    buf->y1 = y;

    return y;
}

void lpf_butterworth6_design(biquad_z_coefs *coefs, int samplerate,
                             int cutout) {
    float omega = (float)cutout / (float)samplerate;
    for (int i = 0; i < N_STAGES; i++) {
        bilinear_biquad_design_lpf(coefs + i, s_coefs + i, omega);
    }
}

float lpf_butterworth6_process(biquad_buffer *buf, const biquad_z_coefs *coefs,
                               float x) {

    float y = biquad_process(&buf[0], &coefs[0], x);
    for (int stage = 1; stage < N_STAGES; stage++) {
        y = biquad_process(&buf[stage], &coefs[stage], y);
    }
    return y;
}

float shaitan_function(float x) {
    return 2.5 * atan(0.9 * x) + 2.5 * sqrt(1 - pow(0.9 * x,  2)) - 2.5;
}
