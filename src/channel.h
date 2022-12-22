#pragma once
#include "text.h"
#include "plot.h"

typedef struct Channel {
  SDL_Point position;

  // body
  text_t *channel_number;
  text_t *plot0_name;
  text_t *plot1_name;

  plot_t *plot0;
  plot_t *plot1;
} channel_t;

typedef struct Channels {
  size_t count;
  channel_t **channel;
} channels_t;

channels_t* Channels_init(int32_t count, SDL_Point position);
void Channels_free(channels_t* channels);
channel_t* Channels_chennel(channels_t* channels, uint32_t idx);

channel_t *Channel_init(SDL_Point position, int32_t channel_number,
                        char const *plot0_name, char const *plot1_name);
void Channel_free(channel_t *channel);
