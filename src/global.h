#pragma once
#include "graphics.h"
#include <stdbool.h>

extern graphics_t   *g_graphics;
extern SDL_Window   *window;
extern SDL_Renderer *renderer;
extern bool          g_graphics_ready;
extern int32_t const g_plots_count;
