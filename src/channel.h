#pragma once
#include "text.h"
#include "plot.h"

typedef struct Channel {
  SDL_Point position;

  // body
  text_t *plot0_name;
  text_t *plot1_name;

  plot_t *plot0;
  plot_t *plot1;
} channel_t;

typedef struct ChannelService {
  channel_t channel;
  text_t *channel_number;
} channel_service_t;

typedef struct ChannelRelay {
  channel_t channel;
  SDL_Texture *channel_number;
  SDL_Rect channel_number_pos;
} channel_relay_t;

typedef struct Channels {
  size_t count;
  channel_t **channel;
} channels_t;

//---------------------------------------------------------------------------------------
channel_t* Channel_relay_init(SDL_Point position, int32_t channel_number,
                        char const *plot0_name, char const *plot1_name);
void Channel_relay_free(channel_t *channel);
channels_t* Channels_relay_init(int32_t count, SDL_Point position);
void Channels_relay_free(channels_t* channels);
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
channel_t* Channel_service_init(SDL_Point position, int32_t channel_number,
                        char const *plot0_name, char const *plot1_name);
void Channel_service_free(channel_t *channel);
channels_t* Channels_service_init(int32_t count, SDL_Point position);
void Channels_service_free(channels_t* channels);
//---------------------------------------------------------------------------------------
