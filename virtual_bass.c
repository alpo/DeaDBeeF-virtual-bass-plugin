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
    virtual_bass->level = 0.5;

    return (ddb_dsp_context_t *)virtual_bass;
}

void virtual_bass_close(ddb_dsp_context_t *ctx) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;

    // free instance-specific allocations

    free(virtual_bass);
}

void virtual_bass_reset(ddb_dsp_context_t *ctx) {
    // use this method to flush dsp buffers, reset filters, etc
}

int virtual_bass_process(ddb_dsp_context_t *ctx, float *samples, int nframes,
                         int maxframes, ddb_waveformat_t *fmt, float *r) {
    ddb_virtual_bass_t *virtual_bass = (ddb_virtual_bass_t *)ctx;
    for (int i = 0; i < nframes * fmt->channels; i++) {
        samples[i] *= virtual_bass->level;
    }
    return nframes;
}

const char *virtual_bass_get_param_name(int p) {
    switch (p) {
        case VIRTUAL_BASS_PARAM_LEVEL:
            return "Volume level";
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
        case VIRTUAL_BASS_PARAM_LEVEL:
            snprintf(val, sz, "%f", virtual_bass->level);
            break;
        default:
            fprintf(stderr,
                    "virtual_bass_get_param: invalid param index (%d)\n", p);
    }
}

static const char settings_dlg[] =
    "property \"Volume Level\" spinbtn[0,2,0.1] 0 0.5;\n";

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
    .plugin.website = "http://virtual_bass.com",
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
