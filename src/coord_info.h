#pragma once
#include <SDL2/SDL.h>

struct coord_info {
  SDL_Point pos;

  // body
  SDL_Color color_background;
  struct text *coords;
  float x, y;
};

struct vec_coord_info {
  struct coord_info **coords;
  size_t count;
};

struct coord_info *coord_info_cons(SDL_Point pos, float x, float y);
void coord_info_free(struct coord_info *coord_info);

struct vec_coord_info *vec_coord_info_cons(size_t count);
void vec_coord_info_free(struct vec_coord_info *vec);
