#include "coinf.h"
#include "colorscheme.h"
#include "comfun.h"
#include "text.h"
#include <SDL2/SDL_rect.h>
#include <assert.h>
#include <uchar.h>

struct coinf8 *coinf8_init_coord(SDL_Point pos, float x, float y) {
  struct coinf8 *info = malloc(sizeof *info);
  size_t const size_buff = 100;
  char buff[size_buff];
  sprintf(buff, "(%.2f, %.2f)", x, y);
  info->inf_txt =
      text8_init(text_get_font_type(TEXT_FONT_REGULAR), 20, COLOR_WHITE,
                 (SDL_Point){.x = pos.x, .y = pos.y - 20}, buff);
  info->back = COLOR_BACKGROUND;
  info->inf = (SDL_FPoint){.x = x, .y = y};
  return info;
}

struct coinf16 *coinf16_init_text(SDL_Point pos, char16_t const *text) {
  struct coinf16 *info = malloc(sizeof *info);
  info->inf_txt =
      text16_init(text_get_font_type(TEXT_FONT_REGULAR), 20, COLOR_WHITE,
                  (SDL_Point){.x = pos.x, .y = pos.y - 20}, text);
  info->back = COLOR_BACKGROUND;
  return info;
}

void coinf8_free(struct coinf8 *coinf) {
  assert(coinf);
  text8_free(coinf->inf_txt);
  free(coinf);
}

void coinf16_free(struct coinf16 *coinf) {
  assert(coinf);
  text16_free(coinf->inf_txt);
  free(coinf);
}
