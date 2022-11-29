#pragma once

#include <stdint.h>
#include <stddef.h>

#include "trace.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void draw_trace(trace_t *trace);

void init_sdl_environment(void);
void clear_sdl_environment(void);
void handle_events(void);
