#include "rchannel.h"
#include "comfun.h"
#include "global.h"
#include "plot.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

char const relay_number_1_off[] = "res/rim_1_off.png";
char const relay_number_2_off[] = "res/rim_2_off.png";
char const relay_number_1_on[] = "res/rim_1_on.png";
char const relay_number_2_on[] = "res/rim_2_on.png";

static inline char const *get_relay_num(size_t num);

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

  rchannel->number.texture = SDL_CreateTextureFromSurface(renderer, sur_num);
  if (rchannel->number.texture == NULL) {
    display_error_sdl("Can't create texture from surface sur_num");
    SDL_FreeSurface(sur_num);
    ok = false;
  }

  rchannel->number.value = channel_number;
  rchannel->number.position.w = sur_num->w;
  rchannel->number.position.h = sur_num->h;
  rchannel->dpos = (SDL_Point){0, 0};

  SDL_FreeSurface(sur_num);
  return ok;
}

struct rchannel *rchannel_init(SDL_Point position, int32_t channel_number,
                               char const *plot0_name, char const *plot1_name) {
  struct rchannel *rchannel = malloc(sizeof *rchannel);

  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  SDL_Point pos_plot0_body = {.x = 69 + 48 + pos_num.x, .y = pos_num.y - 25};
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
  if (rchannel == NULL) {
    return;
  }

  plot_free(rchannel->plot0);
  plot_free(rchannel->plot1);
  SDL_DestroyTexture(rchannel->number.texture);
  free(rchannel);
  rchannel = NULL;
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
  vec->rchs = malloc(sizeof(struct rchannel *) * count);

  SDL_Point dpos = position;
  for (size_t i = 0; i < count; ++i) {
    vec->rchs[i] = rchannel_init(dpos, i, "Tx", "Rx");

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return vec;
}

void vec_rchannel_free(struct vec_rchannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {
    if (vec->rchs[i] == NULL)
      continue;
    rchannel_free(vec->rchs[i]);
    vec->rchs[i] = NULL;
  }

  free(vec->rchs);
  vec->rchs = NULL;

  free(vec);
  vec = NULL;
}

void off_rchannel(struct vec_rchannel *vec, int rch_idx) {
  struct rchannel *rch = vec->rchs[rch_idx];
  if (rch->state == false)
    return;

  rch->state = false;

  size_t num = 0;
  switch (rch->number.value) {
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

  rchannel_init_number(rch, num);
}
