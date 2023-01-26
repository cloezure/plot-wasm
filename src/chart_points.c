#include "chart_points.h"
#include "colorscheme.h"
#include "text.h"
#include <assert.h>
#include <stddef.h>

struct chart_points *chart_points_init(char const *unit, SDL_Point pos_unit,
                                       int32_t *charts, size_t len,
                                       SDL_Point pos_chart,
                                       enum CHARTS_MODS charts_mods) {
  struct chart_points *chpo = malloc(sizeof *chpo);
  chpo->unit = text_init(text_get_font_type(TEXT_FONT_BOLD), 12,
                         COLOR_CHANNEL_HEAD, pos_unit, unit);

  chpo->len = len;
  chpo->charts_nums = malloc(sizeof *chpo->charts_nums);
  for (size_t i = 0; i < len; ++i) {
    chpo->charts_nums[i] = charts[i];
  }

  chpo->points = malloc(sizeof(struct text *) * len);
  for (size_t i = 0; i < len; ++i) {
    char buff[64] = {0};
    sprintf(buff, "%3d", charts[i]);
    chpo->points[i] = text_init(text_get_font_type(TEXT_FONT_REGULAR), 14,
                                COLOR_CHANNEL_HEAD, pos_chart, buff);

    if (charts_mods == CHARTS_MODE_H) {
      pos_chart.y += 24;
    } else if (charts_mods == CHARTS_MODE_V) {
      pos_chart.x += 24;
    }
  }

  return chpo;
}

void chart_points_free(struct chart_points *chpo) {
  assert(chpo);

  for (size_t i = 0; i < chpo->len; ++i) {
    text_free(chpo->points[i]);
  }
  free(chpo->points);
  chpo->points = NULL;

  free(chpo->charts_nums);
  chpo->charts_nums = NULL;

  chpo->len = 0;
  text_free(chpo->unit);

  free(chpo);
}
