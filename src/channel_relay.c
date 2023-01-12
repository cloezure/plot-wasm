#include "channel_relay.h"
#include "channel.h"
#include "common_function.h"
#include "global.h"
#include <SDL2/SDL_image.h>

char const relay_number_1_off[] = "res/rim_1_off.png";
char const relay_number_2_off[] = "res/rim_2_off.png";
char const relay_number_1_on[] = "res/rim_1_on.png";
char const relay_number_2_on[] = "res/rim_2_on.png";

static inline char const *get_relay_num(size_t num);

struct channel* channel_relay_build(SDL_Point position,
                                          int32_t channel_number,
                                          char const *plot0_name,
                                          char const *plot1_name) {
  struct channel_relay *new_channel = malloc(sizeof *new_channel);
  new_channel->channel = channel_alloc();
  new_channel->channel_number_count = channel_number;

  // body
  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  channel_build(new_channel->channel, position, plot0_name, plot1_name,
                pos_num);

  SDL_Surface *sur_num = IMG_Load(get_relay_num(channel_number));
  if (sur_num == NULL) {
    display_error_img("Can't load rim_n.png");
    return NULL;
  }

  new_channel->channel_number = SDL_CreateTextureFromSurface(renderer, sur_num);
  if (new_channel->channel_number == NULL) {
    display_error_sdl("Can't create texture from surface sur_num");
    SDL_FreeSurface(sur_num);
    return NULL;
  }
  new_channel->channel_number_pos = (SDL_Rect){
      .w = sur_num->w, .h = sur_num->h, .x = pos_num.x, .y = pos_num.y};

  SDL_FreeSurface(sur_num);

  return (struct channel*)new_channel;
}

void channel_relay_free(struct channel_relay *channel) {
  if (channel == NULL) {
    return;
  }

  struct channel_relay *rchannel = (struct channel_relay *)channel;
  channel_free(rchannel->channel);

  SDL_DestroyTexture(rchannel->channel_number);
  free(rchannel);
  channel = NULL;
}

void channel_relay_switch_number(struct channel_relay *channel) {

  size_t num = 0;
  switch (channel->channel_number_count) {
  case 0:
    num = 2;
    break;
  case 1:
    num = 3;
    break;
  case 2:
    num = 0;
    break;
  case 3:
    num = 1;
    break;
  }

  SDL_Surface *sur_num = IMG_Load(get_relay_num(num));
  if (sur_num == NULL) {
    display_error_img("Can't load rim_n.png");
    return;
  }

  if (channel->channel_number != NULL) {
    SDL_DestroyTexture(channel->channel_number);
    channel->channel_number = NULL;
  }

  channel->channel_number = SDL_CreateTextureFromSurface(renderer, sur_num);
  if (channel->channel_number == NULL) {
    display_error_sdl("Can't create texture from surface sur_num");
    SDL_FreeSurface(sur_num);
    return;
  }

  SDL_FreeSurface(sur_num);
}

static inline char const *get_relay_num(size_t num) {
  switch (num) {
  case 0:
    return relay_number_1_on;
  case 1:
    return relay_number_2_on;
  case 2:
    return relay_number_1_off;
  case 3:
    return relay_number_2_off;
  default:
    return "";
  }
}
