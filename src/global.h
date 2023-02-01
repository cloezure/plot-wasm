#pragma once
#include "graphics.h"
#include "lang.h"
#include <stdbool.h>

extern struct graphics *g_graphics;
extern SDL_Window *g_window;
extern SDL_Renderer *g_renderer;
extern struct lang *g_lang;
extern int g_last_press;

#define PLOTS_COUNT 12
#define CHANNELS_COUNT 6
