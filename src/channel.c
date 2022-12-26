#include "channel.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"
#include "plot.h"
#include "text.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

enum TYPE_CHANNEL_RELAY {
TYPE_RELAY_1 = 0,
TYPE_RELAY_2,
};

static inline char const* get_name_num(enum TYPE_CHANNEL_RELAY type) {
  switch (type) {
    case TYPE_RELAY_1: {
      return "res/rim_1.png";
      break;
    }
    case TYPE_RELAY_2: {
      return "res/rim_2.png";
      break;
    }
  }
}

channel_t *Channel_init(SDL_Point position, int32_t channel_number,
                        char const *plot0_name, char const *plot1_name) {
  channel_t *new_channel = malloc(sizeof(*new_channel));
  new_channel->position = position;

  char channel_number_s[100];
  sprintf(channel_number_s, "%d", channel_number);

  // body
  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  new_channel->channel_number = Text_init(font_type(FONT_B), 150, COLOR_CHANNEL_NUMBER,
                                          pos_num, channel_number_s);

  // plots
  SDL_Point pos_plot0_body = {.x = 69 + 48 + pos_num.x,
                              .y = pos_num.y - 25};
  new_channel->plot0 = Plot_init(pos_plot0_body);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x + new_channel->plot0->position.w + 73,
                              .y = pos_num.y - 25};
  new_channel->plot1 = Plot_init(pos_plot1_body);

  int32_t const plot_size_name = 20;
  SDL_Rect pos_plot0_name = {.x = pos_plot0_body.x + 9, .y = pos_plot0_body.y - 11};
  new_channel->plot0_name = Text_init(font_type(FONT_B), plot_size_name, COLOR_PLOT_NAME,
                                      pos_plot0_name, plot0_name);

  SDL_Rect pos_plot1_name = {.x = pos_plot0_name.x + 280, .y = pos_plot0_name.y};
  new_channel->plot1_name = Text_init(font_type(FONT_B), plot_size_name, COLOR_PLOT_NAME,
                                      pos_plot1_name, plot1_name);

  return new_channel;
}

void Channel_free(channel_t *channel) {
  if (channel == NULL) {
    return;
  }

  Text_free(channel->channel_number);
  Text_free(channel->plot0_name);
  Text_free(channel->plot1_name);

  Plot_free(channel->plot0);
  Plot_free(channel->plot1);

  free(channel);
  channel = NULL;
}

channels_t* Channels_init(int32_t count, SDL_Point position) {
  channels_t *channels = malloc(sizeof(*channels));
  channels->count = count;
  channels->channel = malloc(sizeof(channel_t) * count);

  SDL_Point dpos = position;

  __REPEAT__(count) {
    channels->channel[i] = Channel_init(dpos, i + 1, "Tx", "Rx");

    if (!iseven(i)) {
      dpos.y += 244;
      dpos.x -= 654*2;
    }

    dpos.x += 654;
  }

  return channels;
}

channel_t* Channels_chennel(channels_t* channels, uint32_t idx) {
  if(idx > channels->count) {
    puts("Error index in channels");
    return NULL;
  }

  return channels->channel[idx];
}

void Channels_free(channels_t* channels) {
  if(channels == NULL) {
    return;
  }

  __REPEAT__(channels->count) {
    if (channels->channel[i] == NULL) continue;
    Channel_free(channels->channel[i]);
    channels->channel[i] = NULL;
  }

  free(channels->channel);
  channels->channel = NULL;

  free(channels);
  channels = NULL;
}

channel_relay_t* Channel_relay_init(SDL_Point position, int32_t channel_number,
                                    char const* plot0_name, char const* plot1_name) {
  channel_relay_t *new_channel = malloc(sizeof(*new_channel));
  new_channel->position = position;

  // body
  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  SDL_Surface *sur_num = IMG_Load(get_name_num(channel_number));
  if (sur_num == NULL ) {
    display_error_img("Can't load rim_n.png");
    return NULL;
  }

  new_channel->channel_number = SDL_CreateTextureFromSurface(renderer, sur_num);
  if (new_channel->channel_number == NULL) {
    display_error_sdl("Can't create texture from surface sur_num");
    SDL_FreeSurface(sur_num);
    return NULL;
  }
  new_channel->channel_number_pos = (SDL_Rect) {
  .w = sur_num->w,
  .h = sur_num->h,
  .x = pos_num.x,
  .y = pos_num.y};

  SDL_FreeSurface(sur_num);

  // plots
  SDL_Point pos_plot0_body = {.x = 69 + 48 + pos_num.x,
                              .y = pos_num.y - 25};
  new_channel->plot0 = Plot_init(pos_plot0_body);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x + new_channel->plot0->position.w + 73,
                              .y = pos_num.y - 25};
  new_channel->plot1 = Plot_init(pos_plot1_body);

  int32_t const plot_size_name = 20;
  SDL_Rect pos_plot0_name = {.x = pos_plot0_body.x + 9, .y = pos_plot0_body.y - 11};
  new_channel->plot0_name = Text_init(font_type(FONT_B), plot_size_name, COLOR_PLOT_NAME,
                                      pos_plot0_name, plot0_name);

  SDL_Rect pos_plot1_name = {.x = pos_plot0_name.x + 280, .y = pos_plot0_name.y};
  new_channel->plot1_name = Text_init(font_type(FONT_B), plot_size_name, COLOR_PLOT_NAME,
                                      pos_plot1_name, plot1_name);

  return new_channel;
}

void Channel_relay_free(channel_relay_t *channel) {
  if (channel == NULL) {
    return;
  }

  SDL_DestroyTexture(channel->channel_number);
  Text_free(channel->plot0_name);
  Text_free(channel->plot1_name);

  Plot_free(channel->plot0);
  Plot_free(channel->plot1);

  free(channel);
  channel = NULL;
}

channels_relay_t* Channels_relay_init(int32_t count, SDL_Point position) {
  channels_relay_t *channels = malloc(sizeof(*channels));
  channels->count = count;
  channels->channel = malloc(sizeof(channel_t) * count);

  SDL_Point dpos = position;

  __REPEAT__(count) {
    channels->channel[i] = Channel_relay_init(dpos, i, "Tx", "Rx");

    if (!iseven(i)) {
      dpos.y += 244;
      dpos.x -= 654*2;
    }

    dpos.x += 654;
  }

  return channels;
}

void Channels_relay_free(channels_relay_t* channels) {
  if(channels == NULL) {
    return;
  }

  __REPEAT__(channels->count) {
    if (channels->channel[i] == NULL) continue;
    Channel_relay_free(channels->channel[i]);
    channels->channel[i] = NULL;
  }

  free(channels->channel);
  channels->channel = NULL;

  free(channels);
  channels = NULL;
}
