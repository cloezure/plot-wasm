#include "coord_info.h"
#include "colorscheme.h"
#include "text.h"

struct coord_info *coord_info_cons(SDL_Point pos, float x, float y) {
  struct coord_info *info = malloc(sizeof *info);
  info->pos = pos;

  size_t const size_buff = 100;
  char buff[size_buff];
  snprintf(buff, size_buff, "%f %f", x, y);
  info->coords = text_cons(text_get_font_type(TEXT_FONT_REGULAR), 10,
                           COLOR_WHITE, pos, buff);
  info->color_background = COLOR_GREEN;
  return info;
}

void coord_info_free(struct coord_info *coord_info) {
  text_free(coord_info->coords);
  free(coord_info);
  coord_info = NULL;
}
