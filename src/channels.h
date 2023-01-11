#pragma once
#include <SDL2/SDL.h>

struct channels;

struct channels *channels_relay_build(size_t count, SDL_Point position);
void channels_relay_free(struct channels *channels);
struct channels *channels_service_build(size_t count, SDL_Point position);
void channels_service_free(struct channels *channels);
