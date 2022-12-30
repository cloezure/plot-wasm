#pragma once
#include "plot.h"
#include "text.h"

typedef struct Channel {
  SDL_Point position;
  // body
  Text *plot0_name;
  Text *plot1_name;

  Plot *plot0;
  Plot *plot1;
} Channel;

typedef struct ChannelService {
  Channel *channel;
  Text *channel_number;
} Channel_service;

typedef struct ChannelRelay {
  Channel *channel;
  SDL_Texture *channel_number;
  SDL_Rect channel_number_pos;
} Channel_relay;

typedef struct Channels {
  size_t count;
  Channel **channel;
} Channels;

//---------------------------------------------------------------------------------------
Channel *Channel_relay_init(SDL_Point position, int32_t channel_number,
                            char const *plot0_name, char const *plot1_name);
void Channel_relay_free(Channel *channel);
Channels *Channels_relay_init(int32_t count, SDL_Point position);
void Channels_relay_free(Channels *channels);
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
Channel *Channel_service_init(SDL_Point position, int32_t channel_number,
                              char const *plot0_name, char const *plot1_name);
void Channel_service_free(Channel *channel);
Channels *Channels_service_init(int32_t count, SDL_Point position);
void Channels_service_free(Channels *channels);
//---------------------------------------------------------------------------------------
