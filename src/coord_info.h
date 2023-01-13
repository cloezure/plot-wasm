#pragma once
#include <SDL2/SDL.h>

struct coord_info {
    SDL_Point pos;
    //body
    SDL_Color color_background;
    struct text* coords;
    float x, y;
};

struct coord_info_vec {
    struct coord_info* coords;
    size_t count;
};

struct coord_info* coord_info_build(SDL_Point pos, float x, float y);
void coord_info_free(struct coord_info* coord_info);

struct coord_info_vec* coord_info_vec_build(size_t count);
void coord_info_vec_free(struct coord_info_vec* coord_info_vec);
