#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

struct channels {
  struct channel **channels;
  size_t channels_count;
  bool *states;
};

struct channels *channels_relay_build(size_t count, SDL_Point position);
void channels_relay_free(struct channels *channels);

struct channels *channels_service_build(size_t count, SDL_Point position);
void channels_service_free(struct channels *channels);
