#pragma once
#include "graphics.h"
#include <stdbool.h>

extern struct graphics *g_graphics;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern bool g_graphics_ready;
extern size_t const g_plots_count;
