#include "chart_points.h"
#include "colorscheme.h"
#include "comfun.h"
#include "text.h"
#include <SDL2/SDL_power.h>
#include <assert.h>
#include <stddef.h>
#include <wchar.h>

struct chart_points *chart_points_init(char16_t const *unit, int32_t *charts,
                                       size_t len, SDL_Point pos_chart,
                                       enum CHARTS_MODS charts_mod) {
  struct chart_points *chpo = malloc(sizeof *chpo);

  chpo->len = len;
  chpo->charts_nums = malloc(sizeof *chpo->charts_nums);
  for (size_t i = 0; i < len; ++i) {
    chpo->charts_nums[i] = charts[i];
  }

  chpo->points = malloc(sizeof(struct text *) * len);
  for (size_t i = 0; i < len; ++i) {
    char buff[64] = {0};
    sprintf(buff, "%3d", charts[i]);
    chpo->points[i] = text8_init(text_get_font_type(TEXT_FONT_REGULAR), 14,
                                 COLOR_CHANNEL_HEAD, pos_chart, buff);

    if (charts_mod == CHARTS_MODE_H) {
      pos_chart.x += 24;
    } else if (charts_mod == CHARTS_MODE_V) {
      pos_chart.y += 24;
    }
  }

  SDL_Rect mid_poings = chpo->points[len / 2]->position;
  SDL_Point unit_pos = {0, 0};
  if (charts_mod == CHARTS_MODE_H) {
    unit_pos = (SDL_Point){.x = mid_poings.x, .y = mid_poings.y - 24};
  } else if (charts_mod == CHARTS_MODE_V) {
    unit_pos = (SDL_Point){.x = mid_poings.x - 24, .y = mid_poings.y};
  }

  chpo->unit = text16_init(text_get_font_type(TEXT_FONT_BOLD), 12,
                           COLOR_CHANNEL_HEAD, unit_pos, unit);

  return chpo;
}

void chart_points_free(struct chart_points *chpo) {
  assert(chpo);

  for (size_t i = 0; i < chpo->len; ++i) {
    text8_free(chpo->points[i]);
  }
  free(chpo->points);
  chpo->points = NULL;

  free(chpo->charts_nums);
  chpo->charts_nums = NULL;

  chpo->len = 0;
  text16_free(chpo->unit);

  free(chpo);
}
