#pragma once

#include <stddef.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#ifde __EMSCRIPTEN__
#include <emscripten.h>
#endif

typedef struct Plots {
} plots_t;

plots_t *Plots_init(int32_t width, int32_t height, int32_t fps);
void Plots_free(plots_t *plot);

bool init(void);
bool load(void);
void quit(void);
