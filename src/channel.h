#pragma once
#include "plot.h"
#include "text.h"

#define RELAY_NUMBER_1_OFF "res/rim_1_off.png"
#define RELAY_NUMBER_2_OFF "res/rim_2_off.png"
#define RELAY_NUMBER_1_ON  "res/rim_1_on.png"
#define RELAY_NUMBER_2_ON  "res/rim_2_on.png"

struct channel {
  SDL_Point position;

  // body
  struct text *plot0_name;
  struct text *plot1_name;

  struct plot *plot0;
  struct plot *plot1;
  bool state;
};

struct channel_service {
  struct channel *channel;
  struct text *channel_number;
};

struct channel *channel_service_build(SDL_Point position,
                                         int32_t channel_number,
                                         char const *plot0_name,
                                         char const *plot1_name);
void channel_service_free(struct channel *channel);

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
void channel_relay_free(struct channel *channel);
void channel_relay_switch_number(struct channel_relay* channel);

struct channels {
  struct channel **channels;
  size_t channels_count;
  bool *states;
};

struct channels *channels_relay_build(size_t count, SDL_Point position);
void channels_relay_free(struct channels *channels);
struct channels *channels_service_build(size_t count, SDL_Point position);
void channels_service_free(struct channels *channels);
