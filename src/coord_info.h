#pragma once
#include <SDL2/SDL.h>

struct coord_info {
    SDL_Rect pos;
    SDL_Color color_background;
    struct text* coords;
};

struct coord_info* coord_info_build(void);
void coord_info_free(struct coord_info* coord_info);
