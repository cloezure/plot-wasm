#include "spectrum.h"
#include <assert.h>

spec_t* Spectrum_init(size_t const fft_count) {
    spec_t * spec = malloc(sizeof(*spec));
    spec->dx = 0.0f;
    spec->x0 = 0.0f;
    spec->fft_reports = malloc(sizeof(*spec->fft_reports) * fft_count);

    return spec;
}

void Spectrum_free(spec_t * spec) {
    assert(spec != NULL);
    assert(spec->fft_reports != NULL);

    free(spec->fft_reports);
    spec->fft_reports = NULL;

    free(spec);
    spec = NULL;
}
