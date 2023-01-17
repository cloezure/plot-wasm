#pragma once
#include "text.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

struct plot {
  SDL_Rect position;
  SDL_Texture *background;
  struct text *name;
  float scale;

  struct {
    float *data;
    size_t length;
    float dx;
    float x0;
  } fft;
};

struct plot *plot_cons(SDL_Point position, char const *name);
void plot_free(struct plot *plot);

void plot_fft_update(struct plot *plot, float *data, int length, float dx,
                     float x0);
