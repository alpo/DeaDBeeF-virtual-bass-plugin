/*
    Virtual bass plugin for DeaDBeeF
    Copyright (C) 2015 Alexey Porotnikov <alexey.porotnikov@gmail.com>
    The MIT License (MIT)
*/

#include "virtual_bass.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>


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

int main() {
    test_open_close();
    test_set_get();
}
