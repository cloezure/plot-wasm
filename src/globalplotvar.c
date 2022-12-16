#include "globalplotvar.h"

int32_t const WIN_WIDTH = 400*2;
int32_t const WIN_HEIGHT = 400*2;
int32_t const WIN_MID_WIDTH = WIN_WIDTH/2;
int32_t const WIN_MID_HEIGHT = WIN_HEIGHT/2;
int32_t FPS_PLOT = 60;

SDL_Window   * g_win         = NULL;
SDL_Renderer * g_ren         = NULL;
coordsinfo_t * g_coords_info = NULL;
bool g_draw_plot = false;
