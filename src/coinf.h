#pragma once
#include <SDL2/SDL.h>

struct coinf {
  SDL_Point pos;
  SDL_FPoint inf;

  // body
  SDL_Color back;
  struct text *inf_txt;
};

struct coinf *coinf_init(SDL_Point pos, float x, float y);
void coinf_free(struct coinf *coinf);
