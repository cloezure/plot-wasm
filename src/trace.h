#pragma once
#include <stdlib.h>
/* #include <stddef.h> */

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct Trace {
    float x0;
    float dx;
    float *fft_reports;
    size_t fft_reports_size;
} trace_t;

void Trace_init(float x0, float dx, float *fft_reports, size_t fft_reports_size);
void Trace_free(trace_t * trace);

/* #ifdef __EMSCRIPTEN__ */
/* EMSCRIPTEN_BINDINGS(my_value) { */
/*     value_object<struct Trace>("Trace") */
/*         .field("x0"              , offsetof(struct Trace, x0)) */
/*         .field("dx"              , offsetof(struct Trace, dx)) */
/*         .field("fft_reports"     , offsetof(struct Trace, fft_reports)) */
/*         .field("fft_reports_size", offsetof(struct Trace, fft_reports_size)) */
/*     ; */

/*     function("Trace_init", &Trace_init); */
/*     function("Trace_free", &Trace_free); */
/* } */
/* #endif */
