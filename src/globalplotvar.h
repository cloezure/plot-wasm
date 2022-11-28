#pragma once

#include "coordsinfo.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <stdint.h>

#define WIN_WIDTH 400
#define WIN_HEIGHT 400
#define WIN_MID_WIDTH WIN_WIDTH/2
#define WIN_MID_HEIGHT WIN_HEIGHT/2

extern SDL_Window   * g_win;
extern SDL_Renderer * g_ren;
extern coordsinfo_t * g_coords_info;

#define DISPLAY_ERROR_SDL(info) \
    printf("%s SDL_Error: %s\n", \
           info, \
           SDL_GetError())

#define DISPLAY_ERROR_TTF(info) \
    printf("%s TTF_Error: %s\n", \
           info, \
           TTF_GetError())