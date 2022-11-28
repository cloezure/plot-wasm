#pragma once
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS()
#endif

typedef struct Trace {
    float x0;
    float dx;
    float *fft_reports;
} trace_t;



trace_t* Spectrum_init();
void Spectrum_free(trace_t * spec);
