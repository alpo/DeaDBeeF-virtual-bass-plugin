/*
    Virtual bass plugin for DeaDBeeF
    Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
    The MIT License (MIT)
*/

#ifndef VIRTUAL_BASS_H_
#define VIRTUAL_BASS_H_

#include <deadbeef/deadbeef.h>

enum {
    VIRTUAL_BASS_PARAM_CUTOFF,
    VIRTUAL_BASS_PARAM_LEVEL,
    VIRTUAL_BASS_PARAM_COUNT
};

#define N_STAGES 3
#define N_CHANNELS 2

typedef struct {
    float a0, a1, a2, b0, b1, b2;
} biquad_z_coefs;

typedef struct {
    float A, B, C, D, E, F;
} biquad_s_coefs;

typedef struct {
    float x1, x2, y1, y2;
} biquad_buffer;

typedef struct {
    ddb_dsp_context_t ctx;

    int samplerate;
    biquad_z_coefs coefs[N_STAGES];
    biquad_buffer lpf_buffer[N_CHANNELS][N_STAGES];

    int cutoff;
    float level;
} ddb_virtual_bass_t;

ddb_dsp_context_t *virtual_bass_open(void);
void virtual_bass_close(ddb_dsp_context_t *ctx);
int virtual_bass_num_params(void);
void virtual_bass_set_param(ddb_dsp_context_t *ctx, int p, const char *val);
void virtual_bass_get_param(ddb_dsp_context_t *ctx, int p, char *val, int sz);

void bilinear_biquad_design_lpf(biquad_z_coefs *out, const biquad_s_coefs *in,
                                float omega);
void biquad_reset(biquad_buffer *buf);
float biquad_process(biquad_buffer *buf, const biquad_z_coefs *coefs, float x);
void lpf_butterworth6_design(biquad_z_coefs *coefs, int samplerate, int cutout);
float lpf_butterworth6_process(biquad_buffer *buf, const biquad_z_coefs *coefs,
                               float x);
float shaitan_function(float x);

#endif  // VIRTUAL_BASS_H_
