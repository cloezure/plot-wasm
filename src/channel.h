#pragma once
#include "text.h"

typedef struct Channel {
  SDL_Point position;

  // head
  SDL_Rect head_background;
  text_t *head_text;

  // body
  text_t *channel_number;
  text_t *plot0_name;
  text_t *plot1_name;


  // plot0
  // plot1
} channel_t;

channel_t *Channel_init(SDL_Point position, char const* head_text, char const *channel_number,
                        char const *plot0_name, char const *plot1_name);
void Channel_free(channel_t *channel);
