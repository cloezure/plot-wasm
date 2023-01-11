#pragma once
#include <SDL2/SDL.h>

struct channel_service;

struct channel_service *channel_service_build(SDL_Point position,
                                              int32_t channel_number,
                                              char const *plot0_name,
                                              char const *plot1_name);
void channel_service_free(struct channel_service *channel);
