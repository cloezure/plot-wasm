#include "relay_channel.h"
#include "common_functions.h"
#include "global_vars.h"
#include "plot.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>

char const relay_number_1_off[] = "res/rim_1_off.png";
char const relay_number_2_off[] = "res/rim_2_off.png";
char const relay_number_1_on[] = "res/rim_1_on.png";
char const relay_number_2_on[] = "res/rim_2_on.png";

static inline char const *get_relay_num(size_t num);
static inline int32_t swap_rchanne_number_value(int32_t value);

static inline bool rchannel_init_number(struct rchannel *rchannel,
                                        int32_t channel_number) {
  bool ok = true;

  SDL_Surface *sur_num = IMG_Load(get_relay_num(channel_number));
  if (sur_num == NULL) {
    display_error_img("Can't load rim_n.png");
    ok = false;
  }

  if (rchannel->number.texture != NULL) {
    SDL_DestroyTexture(rchannel->number.texture);
  }

  rchannel->number.texture = SDL_CreateTextureFromSurface(g_renderer, sur_num);
  if (rchannel->number.texture == NULL) {
    display_error_sdl("Can't create texture from surface sur_num");
    SDL_FreeSurface(sur_num);
    ok = false;
  }

  rchannel->number.value = channel_number;
  rchannel->number.position.w = sur_num->w;
  rchannel->number.position.h = sur_num->h;

  SDL_FreeSurface(sur_num);
  return ok;
}

struct rchannel *rchannel_init(SDL_Point position, int32_t channel_number,
                               char16_t const *plot0_name,
                               char16_t const *plot1_name) {
  struct rchannel *rchannel = malloc(sizeof *rchannel);

  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  SDL_Point pos_plot0_body = {.x = 130 + pos_num.x, .y = pos_num.y - 25};
  rchannel->plot0 = plot_init(pos_plot0_body, plot0_name);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x +
                                   rchannel->plot0->position.w + 73,
                              .y = pos_num.y - 25};
  rchannel->plot1 = plot_init(pos_plot1_body, plot1_name);
  rchannel->state = true;

  rchannel->number.position.x = pos_num.x;
  rchannel->number.position.y = pos_num.y;
  rchannel->number.texture = NULL;
  if (!rchannel_init_number(rchannel, channel_number))
    return NULL;

  return rchannel;
}

void rchannel_free(struct rchannel *rchannel) {
  assert(rchannel);

  plot_free(rchannel->plot0);
  plot_free(rchannel->plot1);
  SDL_DestroyTexture(rchannel->number.texture);
  free(rchannel);
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
    return relay_number_2_off;
  }
}

struct vec_rchannel *vec_rchannel_init(size_t count, SDL_Point position) {
  struct vec_rchannel *vec = malloc(sizeof *vec);
  vec->count = count;
  vec->channels = malloc(sizeof(struct rchannel *) * count);

  SDL_Point dpos = position;
  for (size_t i = 0; i < count; ++i) {
    vec->channels[i] = rchannel_init(dpos, i, u"Tx", u"Rx");

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return vec;
}

void vec_rchannel_free(struct vec_rchannel *vec) {
  assert(vec);

  for (size_t i = 0; i < vec->count; ++i) {
    rchannel_free(vec->channels[i]);
    vec->channels[i] = NULL;
  }

  free(vec->channels);
  vec->channels = NULL;

  free(vec);
}

static inline int32_t swap_rchanne_number_value(int32_t value) {
  switch (value) {
  case 0:
    return 2;
    break;
  case 1:
    return 3;
    break;
  case 2:
    return 0;
    break;
  case 3:
    return 1;
    break;
  default:
    return 0;
  }
}

void off_rchannel(struct vec_rchannel *vec, int rch_idx) {
  struct rchannel *channel = vec->channels[rch_idx];
  if (channel->state == false)
    return;

  channel->state = false;
  rchannel_init_number(channel,
                       swap_rchanne_number_value(channel->number.value));
}

void on_rchannel(struct vec_rchannel *vec, int rch_idx) {
  struct rchannel *channel = vec->channels[rch_idx];
  if (channel->state == true)
    return;

  channel->state = true;
  rchannel_init_number(channel,
                       swap_rchanne_number_value(channel->number.value));
}
