#pragma once
#include "graphics.h"
#include <stdbool.h>

extern struct graphics *g_graphics;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern size_t const g_plots_count;
extern SDL_Point mouse;
