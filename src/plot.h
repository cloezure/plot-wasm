#pragma once

#include <stdint.h>
#include <stddef.h>

#include "spectrum.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_spec(float x0, float dx, float* reports);

void init_sdl_environment(void);
void clear_sdl_environment(void);
void handle_events(void);
