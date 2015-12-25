/*
    Virtual bass plugin for DeaDBeeF
    Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
    The MIT License (MIT)
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <deadbeef/deadbeef.h>

#include "virtual_bass.h"

static DB_functions_t *deadbeef;
static DB_dsp_t plugin;

ddb_dsp_context_t *virtual_bass_open(void) {
    ddb_virtual_bass_t *virtual_bass = malloc(sizeof(ddb_virtual_bass_t));
    DDB_INIT_DSP_CONTEXT(virtual_bass, ddb_virtual_bass_t, &plugin);

    // initialize
    virtual_bass->cutoff = 250;
    virtual_bass->level = 1.0;
    virtual_bass->samplerate = 0;

    return (ddb_dsp_context_t *)virtual_bass;
}

void virtual_bass_close(ddb_dsp_context_t *ctx) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;

    // free instance-specific allocations

    free(virtual_bass);
}

void virtual_bass_reset(ddb_dsp_context_t *ctx) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;

    for (int channel = 0; channel < 2; channel++) {
        for (int stage = 0; stage < 3; stage++) {
            biquad_reset(&virtual_bass->lpf_buffer[channel][stage]);
        }
    }
}

int virtual_bass_process(ddb_dsp_context_t *ctx, float *samples, int nframes,
                         int maxframes, ddb_waveformat_t *fmt, float *r) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;

    if (fmt->channels != 2) return nframes;

    if (fmt->samplerate != virtual_bass->samplerate) {
        virtual_bass->samplerate = fmt->samplerate;
        lpf_butterworth6_design(virtual_bass->coefs, virtual_bass->samplerate,
                                virtual_bass->cutoff);
    }

    for (int i = 0; i < nframes; i++) {
        float x0 = samples[2 * i];
        float x1 = samples[2 * i + 1];
        float lf0 = lpf_butterworth6_process(virtual_bass->lpf_buffer[0],
                                             virtual_bass->coefs, x0);
        float lf1 = lpf_butterworth6_process(virtual_bass->lpf_buffer[1],
                                             virtual_bass->coefs, x1);
        float lf = (lf0 + lf1) / 2.0;
        float lfs =
            shaitan_function(lf * virtual_bass->level) / virtual_bass->level;
        samples[2 * i] = x0 - lf0 + lfs;
        samples[2 * i + 1] = x1 - lf1 + lfs;
    }
    return nframes;
}

const char *virtual_bass_get_param_name(int p) {
    switch (p) {
        case VIRTUAL_BASS_PARAM_CUTOFF:
            return "Crossover cutoff";
        case VIRTUAL_BASS_PARAM_LEVEL:
            return "Effect level";
        default:
            fprintf(stderr,
                    "virtual_bass_param_name: invalid param index (%d)\n", p);
    }
    return NULL;
}

int virtual_bass_num_params(void) { return VIRTUAL_BASS_PARAM_COUNT; }

void virtual_bass_set_param(ddb_dsp_context_t *ctx, int p, const char *val) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;
    switch (p) {
        case VIRTUAL_BASS_PARAM_CUTOFF:
            virtual_bass->cutoff = atoi(val);
            virtual_bass_reset(ctx);
            lpf_butterworth6_design(virtual_bass->coefs,
                                    virtual_bass->samplerate,
                                    virtual_bass->cutoff);
            break;
        case VIRTUAL_BASS_PARAM_LEVEL:
            virtual_bass->level = atof(val);
            break;
        default:
            fprintf(stderr, "virtual_bass_param: invalid param index (%d)\n",
                    p);
    }
}

void virtual_bass_get_param(ddb_dsp_context_t *ctx, int p, char *val, int sz) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;
    switch (p) {
        case VIRTUAL_BASS_PARAM_CUTOFF:
            snprintf(val, sz, "%d", virtual_bass->cutoff);
            break;
        case VIRTUAL_BASS_PARAM_LEVEL:
            snprintf(val, sz, "%f", virtual_bass->level);
            break;
        default:
            fprintf(stderr,
                    "virtual_bass_get_param: invalid param index (%d)\n", p);
    }
}

static const char settings_dlg[] =
    "property \"Crossover cutoff\" hscale[50,500,10] 0 1;"
    "property \"Effect level\" hscale[0.3,3,0.1] 1 1;\n";

static DB_dsp_t plugin = {
    .plugin.api_vmajor = DB_API_VERSION_MAJOR,
    .plugin.api_vminor = DB_API_VERSION_MINOR,
    .open = virtual_bass_open,
    .close = virtual_bass_close,
    .process = virtual_bass_process,
    .plugin.version_major = 0,
    .plugin.version_minor = 1,
    .plugin.type = DB_PLUGIN_DSP,
    .plugin.id = "virtual_bass",
    .plugin.name = "Virtual bass",
    .plugin.descr = "Virtual bass DSP Plugin",
    .plugin.copyright =
        "Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>",
    .plugin.website = "https://github.com/alpo/DeaDBeeF-virtual-bass-plugin",
    .num_params = virtual_bass_num_params,
    .get_param_name = virtual_bass_get_param_name,
    .set_param = virtual_bass_set_param,
    .get_param = virtual_bass_get_param,
    .reset = virtual_bass_reset,
    .configdialog = settings_dlg, };

DB_plugin_t *virtual_bass_load(DB_functions_t *f) {
    deadbeef = f;
    return &plugin.plugin;
}
