#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <uchar.h>

struct rchannel {
  SDL_Point position;
  struct plot *plot0;
  struct plot *plot1;
  bool state;

  struct {
    SDL_Texture *texture;
    SDL_Rect position;
    int32_t value;
  } number;
};

struct rchannel *rchannel_init(SDL_Point position, int32_t channel_number,
                               char16_t const *plot0_name,
                               char16_t const *plot1_name);
void rchannel_free(struct rchannel *rchannel);

struct vec_rchannel {
  struct rchannel **channels;
  size_t count;
};

struct vec_rchannel *vec_rchannel_init(size_t count, SDL_Point position);
void vec_rchannel_free(struct vec_rchannel *vec);
void off_rchannel(struct vec_rchannel *vec, int rch_idx);
void on_rchannel(struct vec_rchannel *vec, int rch_idx);
