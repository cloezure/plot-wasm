#pragma once
#include "SDL2/SDL.h"

struct channel_relay {
  struct channel *channel;

  struct {
    SDL_Texture *texture;
    SDL_Rect position;
    int32_t value;
  } channel_number;
};

struct channel *channel_relay_cons(SDL_Point position, int32_t channel_number,
                                   char const *plot0_name,
                                   char const *plot1_name);

void channel_relay_free(struct channel_relay *rchannel);
void channel_relay_switch_number(struct channel_relay *rchannel);
