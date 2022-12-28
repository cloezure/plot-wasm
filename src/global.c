#include "global.h"

graphics_t   *g_graphics       = NULL;
SDL_Window   *window           = NULL;
SDL_Renderer *renderer         = NULL;
bool          g_graphics_ready = false;
int32_t const g_plots_count    = 12;
