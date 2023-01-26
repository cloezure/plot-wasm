#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "plot.h"
#include "text.h"

struct schannel {
  SDL_Point position;
  struct plot *plot0;
  struct plot *plot1;
  struct text *number;
  bool state;
};

struct vec_schannel {
  struct schannel **schs;
  size_t count;
};

struct schannel *schannel_init(SDL_Point position, int32_t channel_number,
                               char const *plot0_name, char const *plot1_name);
void schannel_free(struct schannel *schannel);

struct vec_schannel *vec_schannel_init(size_t count, SDL_Point position);
void vec_schannel_free(struct vec_schannel *vec);
void off_schannel(struct vec_schannel *vec, int sch_idx);
