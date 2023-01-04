#pragma once
#include "plot.h"
#include "text.h"

#define RELAY_NUMBER_1 "res/rim_1.png"
#define RELAY_NUMBER_2 "res/rim_2.png"

struct channel {
  SDL_Point position;
  // body
  struct text *plot0_name;
  struct text *plot1_name;

  struct plot *plot0;
  struct plot *plot1;
};

struct channel_service {
  struct channel *channel;
  struct text *channel_number;
};

struct channel_relay {
  struct channel *channel;
  SDL_Texture *channel_number;
  SDL_Rect channel_number_pos;
};

struct channels {
  struct channel **channels;
  size_t count;
};

//---------------------------------------------------------------------------------------
struct channel *channel_relay_init(SDL_Point position, int32_t channel_number,
                                   char const *plot0_name,
                                   char const *plot1_name);
void channel_relay_free(struct channel *channel);

struct channels *channels_relay_init(size_t count, SDL_Point position);
void channels_relay_free(struct channels *channels);
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
struct channel *channel_service_init(SDL_Point position, int32_t channel_number,
                                     char const *plot0_name,
                                     char const *plot1_name);
void channel_service_free(struct channel *channel);

struct channels *channels_service_init(size_t count, SDL_Point position);
void channels_service_free(struct channels *channels);
//---------------------------------------------------------------------------------------
