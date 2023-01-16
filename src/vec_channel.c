#include "vec_channel.h"
#include "channel.h"
#include "channel_relay.h"
#include "channel_service.h"
#include "common_function.h"

struct vec_channel *vec_channel_service_cons(size_t count, SDL_Point position) {
  struct vec_channel *vec = malloc(sizeof *vec);
  vec->channels = malloc(sizeof(struct channel *) * count);
  vec->count = count;

  SDL_Point dpos = position;
  for (size_t i = 0; i < count; ++i) {
    vec->channels[i] = channel_service_cons(dpos, i + 1, "Tx", "Rx");

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return vec;
}

void vec_channel_service_free(struct vec_channel *vec) {
  if (vec == NULL) {
    return;
  }

  for (size_t i = 0; i < vec->count; ++i) {
    if (vec->channels[i] == NULL)
      continue;
    channel_service_free((struct channel_service *)vec->channels[i]);
    vec->channels[i] = NULL;
  }

  free(vec->channels);
  vec->channels = NULL;

  free(vec);
  vec = NULL;
}

struct vec_channel *vec_channel_relay_cons(size_t count, SDL_Point position) {
  struct vec_channel *vec = malloc(sizeof *vec);
  vec->count = count;
  vec->channels = malloc(sizeof(struct channel *) * count);

  SDL_Point dpos = position;
  for (size_t i = 0; i < count; ++i) {
    vec->channels[i] = channel_relay_cons(dpos, i, "Tx", "Rx");

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return vec;
}

void vec_channel_relay_free(struct vec_channel *vec) {
  if (vec == NULL) {
    return;
  }

  for (size_t i = 0; i < vec->count; ++i) {
    if (vec->channels[i] == NULL)
      continue;
    channel_relay_free((struct channel_relay *)vec->channels[i]);
    vec->channels[i] = NULL;
  }

  free(vec->channels);
  vec->channels = NULL;

  free(vec);
  vec = NULL;
}
