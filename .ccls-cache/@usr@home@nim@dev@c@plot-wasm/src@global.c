#include "global.h"

struct graphics *g_graphics = NULL;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
size_t const g_plots_count = 12;
SDL_Point mouse = { 0, 0};
