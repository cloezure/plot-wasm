#pragma once
#include <SDL2/SDL.h>
#include "llist.h"
#include "plot_draw.h"

typedef struct Plot {
    SDL_Rect position;
    SDL_Texture *background;
    plot_params params;
} plot_t;

plot_t* Plot_init(SDL_Point position);
void Plot_free(plot_t * plot);
