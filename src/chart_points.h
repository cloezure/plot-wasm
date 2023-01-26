#pragma once
#include "text.h"
#include <SDL2/SDL_rect.h>

enum CHARTS_MODS {
  CHARTS_MODE_V = 0,
  CHARTS_MODE_H = 1,
};

struct chart_points {
  struct text **points;
  int32_t *charts_nums;
  size_t len;

  struct text *unit;
};

struct chart_points *chart_points_init(char const *unit, SDL_Point pos_unit,
                                       int32_t *charts, size_t len,
                                       SDL_Point pos_chart,
                                       enum CHARTS_MODS charts_mods);
void chart_points_free(struct chart_points *chart_points);
