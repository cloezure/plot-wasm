#pragma once
#include "text.h"
#include <SDL2/SDL.h>
#include <uchar.h>

struct info_win16 {
  SDL_Color background;
  struct text16 *text;
};

struct info_win8 {
  SDL_Color background;
  struct text8 *text;
};

struct info_win8 *info_win8_init_coord(SDL_Point position, float x, float y);
void info_win8_free(struct info_win8 *coinf);

struct info_win16 *info_win16_init_text(SDL_Point position,
                                        char16_t const *text);
void info_win16_free(struct info_win16 *coinf);
