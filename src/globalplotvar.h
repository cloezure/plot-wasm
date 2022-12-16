#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <stdint.h>
#include <stdbool.h>

#include "coordsinfo.h"

extern int32_t const WIN_WIDTH;
extern int32_t const WIN_HEIGHT;
extern int32_t const WIN_MID_WIDTH;
extern int32_t const WIN_MID_HEIGHT;
extern int32_t       FPS_PLOT;

extern SDL_Window   * g_win;
extern SDL_Renderer * g_ren;
extern coordsinfo_t * g_coords_info;
extern bool           g_draw_plot;

#define DISPLAY_ERROR_SDL(info) \
    printf("%s SDL_Error: %s\n", \
           info, \
           SDL_GetError())

#define DISPLAY_ERROR_TTF(info) \
    printf("%s TTF_Error: %s\n", \
           info, \
           TTF_GetError())
