#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

struct plot;

struct plot *plot_build(SDL_Point position, char const *name);
void plot_free(struct plot *plot);

int plot_get_pos_w(struct plot *plot);
int plot_get_pos_h(struct plot *plot);
int plot_get_pos_x(struct plot *plot);
int plot_get_pos_y(struct plot *plot);
struct text const *plot_get_name(struct plot *plot);
void plot_fft_update(struct plot *plot, float *data, int length, float dx,
                     float x0);
