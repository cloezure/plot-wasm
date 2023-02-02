#pragma once
#include "text.h"
#include <SDL2/SDL_rect.h>

enum CHARTS_MODS {
  CHARTS_MODE_V = 0,
  CHARTS_MODE_H = 1,
};

struct chart_points {
  struct text8 **points;
  int32_t *charts_nums;
  size_t length;

  struct text16 *unit;
};

struct chart_points *chart_points_init(char16_t const *unit, int32_t *charts,
                                       size_t length, SDL_Point pos_chart,
                                       enum CHARTS_MODS charts_mod, size_t gap);
void chart_points_free(struct chart_points *chart_points);
