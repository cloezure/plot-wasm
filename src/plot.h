#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_spec(float* specs, int32_t spec_count, int32_t reports_count, size_t spec_config_size);

void init_sdl_environment(void);
void clear_sdl_environment(void);
void handle_events(void);
