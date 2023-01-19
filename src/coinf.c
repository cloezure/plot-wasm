#include "coinf.h"
#include "colorscheme.h"
#include "text.h"
#include <SDL2/SDL_rect.h>

struct coinf *coinf_init(SDL_Point pos, float x, float y) {
  struct coinf *info = malloc(sizeof *info);
  size_t const size_buff = 100;
  char buff[size_buff];
  snprintf(buff, size_buff, "(%.2f, %.2f)", x, y);
  info->inf_txt =
      text_init(text_get_font_type(TEXT_FONT_REGULAR), 20, COLOR_WHITE,
                (SDL_Point){.x = pos.x, .y = pos.y - 20}, buff);
  info->back = COLOR_BACKGROUND;
  info->inf = (SDL_FPoint){.x = x, .y = y};
  return info;
}

struct coinf *coinf_init_text(SDL_Point pos, char const *text) {
  struct coinf *info = malloc(sizeof *info);
  info->inf_txt =
      text_init(text_get_font_type(TEXT_FONT_REGULAR), 20, COLOR_WHITE,
                (SDL_Point){.x = pos.x, .y = pos.y - 20}, text);
  info->back = COLOR_BACKGROUND;
  return info;
}

void coinf_free(struct coinf *coinf) {
  text_free(coinf->inf_txt);
  free(coinf);
  coinf = NULL;
}
