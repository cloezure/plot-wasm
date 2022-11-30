#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void draw_trace(float x0, float dx, float* fft_reports, int fft_reports_size);

void init_sdl_environment(void);
void clear_sdl_environment(void);
void handle_events(void);
