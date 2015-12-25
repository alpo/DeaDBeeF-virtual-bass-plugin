/*
    Virtual bass plugin for DeaDBeeF
    Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
    The MIT License (MIT)
*/

#include "virtual_bass.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static void test_open_close() {
    ddb_dsp_context_t *ctx;
    ctx = virtual_bass_open();
    virtual_bass_close(ctx);
    assert(true && "Impossible assert");
}

static void test_set_get() {
    ddb_dsp_context_t *ctx;
    ctx = virtual_bass_open();
    virtual_bass_set_param(ctx, VIRTUAL_BASS_PARAM_LEVEL, "0.17");
    char buffer[100];
    virtual_bass_get_param(ctx, VIRTUAL_BASS_PARAM_LEVEL, buffer,
                           sizeof(buffer));
    assert(atof(buffer) == 0.17 && "Set-get test failed");
    virtual_bass_close(ctx);
}

static void test_biquad_design() {
    biquad_s_coefs in;
    biquad_z_coefs out;
    in.D = in.E = 0.0;
    in.A = in.C = in.F = 1.0;
    in.B = 1.9318516526;
    float omega = 0.2;
    bilinear_biquad_design_lpf(&out, &in, omega);
    assert(fabs(out.a0 - 1.0) < 1e-7 && "a0 test failed");
    assert(fabs(out.a1 + 1.032069405) < 1e-7 && "a1 test failed");
    assert(fabs(out.a2 - 0.275707942) < 1e-7 && "a2 test failed");
    assert(fabs(out.b0 - 0.060909634) < 1e-7 && "b0 test failed");
    assert(fabs(out.b1 - 0.121819269) < 1e-7 && "b1 test failed");
    assert(fabs(out.b2 - 0.060909634) < 1e-7 && "b2 test failed");
}

static void test_lpf6_design() {
    biquad_z_coefs coef[3];
    lpf_butterworth6_design(coef, 44100, 250);
    assert(true && "b2 test failed");
}

int main() {
    test_open_close();
    test_set_get();
    test_biquad_design();
    test_lpf6_design();
}
