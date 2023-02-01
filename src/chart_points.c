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
                                       enum CHARTS_MODS charts_mod,
                                       size_t gap) {
  struct chart_points *chpo = malloc(sizeof *chpo);

  chpo->len = len;
  chpo->charts_nums = malloc(sizeof(int32_t) * len);
  for (size_t i = 0; i < len; ++i) {
    chpo->charts_nums[i] = charts[i];
  }

  chpo->points = malloc(sizeof(struct text8 *) * len);
  bool sdvig_1 = true;
  bool sdvig_2 = true;
  bool sdvig_3 = true;
  for (size_t i = 0; i < len; ++i) {
    if (charts_mod == CHARTS_MODE_V) {
      if (charts[i] > -10 && sdvig_1) {
        pos_chart.x += 15;
        sdvig_1 = false;
      }
      if (charts[i] <= -10 && sdvig_2) {
        pos_chart.x -= 8;
        sdvig_2 = false;
      }
      if (charts[i] <= -100 && sdvig_3) {
        pos_chart.x -= 5;
        sdvig_3 = false;
      }
    }

    char buff[64] = {0};
    sprintf(buff, "%+2d", charts[i]);
    chpo->points[i] = text8_init(text_get_font_type(TEXT_FONT_REGULAR), 14,
                                 COLOR_CHANNEL_HEAD, pos_chart, buff);

    if (charts_mod == CHARTS_MODE_V) {
      pos_chart.y += gap;
    } else if (charts_mod == CHARTS_MODE_H) {
      pos_chart.x += gap;
    }
  }

  SDL_Rect mid_poings = chpo->points[len / 2]->position;
  SDL_Point unit_pos = {0, 0};
  if (charts_mod == CHARTS_MODE_H) {
    unit_pos = (SDL_Point){.x = mid_poings.x, .y = mid_poings.y + 15};
  } else if (charts_mod == CHARTS_MODE_V) {
    unit_pos = (SDL_Point){.x = mid_poings.x + 10,
                           .y = mid_poings.y -
                                ((len + 1) * chpo->points[0]->position.h) - 5};
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
