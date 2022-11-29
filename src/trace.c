#include "trace.h"
#include <assert.h>

trace_t* Trace_init(size_t fft_reports_size) {
    trace_t * trace = malloc(sizeof(*trace));
    trace->dx = 0.0f;
    trace->x0 = 0.0f;
    trace->fft_reports_size = fft_reports_size;
    trace->fft_reports = malloc(sizeof(*trace->fft_reports) *
                                trace->fft_reports_size);

    return trace;
}

void Trace_free(trace_t * trace) {
    assert(trace != NULL);
    assert(trace->fft_reports != NULL);

    free(trace->fft_reports);
    trace->fft_reports = NULL;
    trace->fft_reports_size = 0;

    free(trace);
    trace = NULL;
}
