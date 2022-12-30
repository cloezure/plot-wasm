#pragma once
#include <SDL2/SDL.h>

typedef struct Plot {
  SDL_Rect position;
  SDL_Texture *background;

  float *fft;
  int32_t fft_len;
  float dx;
  float x0;
} Plot;

Plot *Plot_init(SDL_Point position);
void Plot_free(Plot *plot);
