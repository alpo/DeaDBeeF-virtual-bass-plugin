/*
    Virtual bass plugin for DeaDBeeF
    Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
    The MIT License (MIT)
*/

#ifndef VIRTUAL_BASS_H_
#define VIRTUAL_BASS_H_

#include <deadbeef/deadbeef.h>

enum {
    VIRTUAL_BASS_PARAM_LEVEL,
    VIRTUAL_BASS_PARAM_COUNT
};

typedef struct {
    ddb_dsp_context_t ctx;
    // instance-specific variables here
    float level;  // this is example
} ddb_virtual_bass_t;

ddb_dsp_context_t *virtual_bass_open(void);
void virtual_bass_close(ddb_dsp_context_t *ctx);
int virtual_bass_num_params(void);
void virtual_bass_set_param(ddb_dsp_context_t *ctx, int p, const char *val);
void virtual_bass_get_param(ddb_dsp_context_t *ctx, int p, char *val, int sz);

#endif  // VIRTUAL_BASS_H_
