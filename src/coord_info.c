#include "coord_info.h"
#include "colorscheme.h"
#include "text.h"

struct coord_info* coord_info_build(SDL_Point pos, float x, float y) {
  struct coord_info* info = malloc(sizeof *info);
  info->pos = pos;

  char buff[10];
  snprintf(buff, 10, "%f %f", x, y);
  info->coords = text_build(text_get_font_type(TEXT_FONT_REGULAR), 10, COLOR_WHITE, pos, buff);
  info->color_background = COLOR_GREEN;
  return info;
}

void coord_info_free(struct coord_info* coord_info) {
    text_free(coord_info->coords);
    free(coord_info);
    coord_info = NULL;
}
