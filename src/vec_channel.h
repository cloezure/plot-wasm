#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

struct vec_channel {
  struct channel **channels;
  size_t count;
};

struct vec_channel *vec_channel_relay_cons(size_t count, SDL_Point position);
void vec_channel_relay_free(struct vec_channel *vec);

struct vec_channel *vec_channel_service_cons(size_t count, SDL_Point position);
void vec_channel_service_free(struct vec_channel *vec);
