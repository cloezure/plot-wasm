#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

#define BMP_BACKGROUND_FOR_PLOT 0
#define PLOT_BACKGROUND_PNG "res/plot_back.png"

struct plot {
  SDL_Rect position;
  SDL_Texture *background;

  struct fft {
    float *data;
    size_t length;
    float dx;
    float x0;
  } fft;
};

struct plot *plot_crealloc(SDL_Point position);
void plot_free(struct plot *plot);
