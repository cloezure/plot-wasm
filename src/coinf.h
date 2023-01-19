#pragma once
#include <SDL2/SDL.h>

struct coinf {
  SDL_FPoint inf;
  SDL_Color back;
  struct text *inf_txt;
};

struct coinf *coinf_init(SDL_Point pos, float x, float y);
struct coinf *coinf_init_text(SDL_Point pos, char const *text);
void coinf_free(struct coinf *coinf);
