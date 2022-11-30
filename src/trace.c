#include "trace.h"
#include "globalplotvar.h"
#include <assert.h>

void Trace_init(float x0, float dx, float *fft_reports, size_t fft_reports_size) {
    trace_t * trace = malloc(sizeof(*trace));
    trace->dx = dx;
    trace->x0 = x0;
    trace->fft_reports_size = fft_reports_size;
    trace->fft_reports = fft_reports;

    g_trace = trace;
}

void Trace_free(trace_t * trace) {
    assert(trace != NULL);
    assert(trace->fft_reports != NULL);

    trace->fft_reports_size = 0;
    free(trace);
    trace = NULL;
}
