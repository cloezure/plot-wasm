#pragma once
#include "text.h"

typedef struct Channel {
  text_t *channel_number;
  text_t *plot0_name;
  text_t *plot1_name;
  SDL_Rect position;
  // plot0
  // plot1
} channel_t;

channel_t *Channel_init(SDL_Rect position, char const *channel_number,
                        char const *plot0_name, char const *plot1_name);
void Channel_free(channel_t *channel);
