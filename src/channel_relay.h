#pragma once
#include "SDL2/SDL.h"

struct channel_relay {
  struct channel *channel;
  SDL_Texture *channel_number;
  SDL_Rect channel_number_pos;
  int32_t channel_number_count;
};

struct channel *channel_relay_build(SDL_Point position,
                                          int32_t channel_number,
                                          char const *plot0_name,
                                          char const *plot1_name);
void channel_relay_free(struct channel_relay *channel);
void channel_relay_switch_number(struct channel_relay *channel);
