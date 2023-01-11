#include "channels.h"
#include "channel.h"
#include "channel_relay.h"
#include "channel_service.h"
#include "common_function.h"
#include <stdbool.h>

struct channels {
  struct channel **channels;
  size_t channels_count;
  bool *states;
};

struct channels *channels_service_build(size_t count, SDL_Point position) {
  struct channels *channels = malloc(sizeof *channels);
  channels->channels_count = count;
  channels->channels = malloc(sizeof(struct channels *) * count);
  channels->states = malloc(sizeof(bool) * count);

  SDL_Point dpos = position;

  for (size_t i = 0; i < count; ++i) {
    channels->channels[i] =
        (struct channel *)channel_service_build(dpos, i + 1, "Tx", "Rx");
    channels->states[i] = channel_get_state(channels->channels[i]);

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return channels;
}

void channels_service_free(struct channels *channels) {
  if (channels == NULL) {
    return;
  }

  for (size_t i = 0; i < channels->channels_count; ++i) {
    if (channels->channels[i] == NULL)
      continue;
    channel_service_free((struct channel_service *)channels->channels[i]);
    channels->channels[i] = NULL;
  }

  free(channels->states);
  free(channels->channels);
  channels->channels = NULL;

  free(channels);
  channels = NULL;
}

struct channels *channels_relay_build(size_t count, SDL_Point position) {
  struct channels *channels = malloc(sizeof *channels);
  channels->channels_count = count;
  channels->channels = malloc(sizeof(struct channel *) * count);
  channels->states = malloc(sizeof(bool) * count);

  SDL_Point dpos = position;

  for (size_t i = 0; i < count; ++i) {
    channels->channels[i] =
        (struct channel_relay *)channel_relay_build(dpos, i, "Tx", "Rx");
    channels->states[i] = channel_get_state(channels->channels[i]);

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return channels;
}

void channels_relay_free(struct channels *channels) {
  if (channels == NULL) {
    return;
  }

  for (size_t i = 0; i < channels->channels_count; ++i) {
    if (channels->channels[i] == NULL)
      continue;
    channel_relay_free((struct channel_relay *)channels->channels[i]);
    channels->channels[i] = NULL;
  }

  free(channels->states);
  free(channels->channels);
  channels->channels = NULL;

  free(channels);
  channels = NULL;
}
