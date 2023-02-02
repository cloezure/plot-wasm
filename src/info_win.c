#include "info_win.h"
#include "colorscheme.h"
#include "common_functions.h"

struct info_win8 *info_win8_init_coord(SDL_Point pos, float x, float y) {
  struct info_win8 *info = malloc(sizeof *info);
  size_t const size_buff = 100;
  char buff[size_buff];
  sprintf(buff, "(%.2f, %.2f)", x, y);
  info->text =
      text8_init(text_get_font_type(TEXT_FONT_REGULAR), 20, COLOR_WHITE,
                 (SDL_Point){.x = pos.x, .y = pos.y - 20}, buff);
  info->background = COLOR_BACKGROUND;
  return info;
}

struct info_win16 *info_win16_init_text(SDL_Point pos, char16_t const *text) {
  struct info_win16 *info = malloc(sizeof *info);
  info->text =
      text16_init(text_get_font_type(TEXT_FONT_REGULAR), 20, COLOR_WHITE,
                  (SDL_Point){.x = pos.x, .y = pos.y - 20}, text);
  info->background = COLOR_BACKGROUND;
  return info;
}

void info_win8_free(struct info_win8 *coinf) {
  text8_free(coinf->text);
  free(coinf);
}

void info_win16_free(struct info_win16 *coinf) {
  text16_free(coinf->text);
  free(coinf);
}
