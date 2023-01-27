#pragma once
#include <SDL2/SDL.h>
#include <uchar.h>

struct coinf16 {
  SDL_FPoint inf;
  SDL_Color back;
  struct text16 *inf_txt;
};

struct coinf8 {
  SDL_FPoint inf;
  SDL_Color back;
  struct text8 *inf_txt;
};

struct coinf8 *coinf8_init_coord(SDL_Point pos, float x, float y);
struct coinf16 *coinf16_init_text(SDL_Point pos, char16_t const *text);
void coinf8_free(struct coinf8 *coinf);
void coinf16_free(struct coinf16 *coinf);
