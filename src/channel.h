#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

struct channel {
  SDL_Point position;
  struct plot *plot0;
  struct plot *plot1;
  bool state;
};

struct channel *channel_cons(SDL_Point position, char const *plot0_name,
                             char const *plot1_name, SDL_Rect pos_num);
void channel_free(struct channel *channel);
