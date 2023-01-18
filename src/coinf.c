#include "coinf.h"
#include "colorscheme.h"
#include "text.h"
#include <SDL2/SDL_rect.h>

struct coinf *coinf_init(SDL_Point pos, float x, float y) {
  struct coinf *info = malloc(sizeof *info);
  info->pos = pos;
  size_t const size_buff = 100;
  char buff[size_buff];
  snprintf(buff, size_buff, "%f %f", x, y);
  info->inf_txt = text_init(text_get_font_type(TEXT_FONT_REGULAR), 10,
                            COLOR_WHITE, pos, buff);
  info->back = COLOR_GREEN;
  info->inf = (SDL_FPoint){.x = x, .y = y};
  return info;
}

void coinf_free(struct coinf *coinf) {
  text_free(coinf->inf_txt);
  free(coinf);
  coinf = NULL;
}
