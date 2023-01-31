#pragma once
#include "chart_points.h"
#include "text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>

struct plot {
  SDL_Rect position;
  SDL_Texture *background;
  struct text16 *name;
  struct chart_points *vcharts;
  struct chart_points *hcharts;
  int index;

  // vertic MHz
  /* struct chart_points *vp; */
  // horiz dB
  /* struct chart_points *hp; */

  struct {
    SDL_FPoint *data;
    size_t length;
    float dx;
    float x0;
  } fft;
};

struct plot *plot_init(SDL_Point position, char16_t const *name);
void plot_free(struct plot *plot);

void plot_fft_update(struct plot *plot, float *data, int length, float dx,
                     float x0);
