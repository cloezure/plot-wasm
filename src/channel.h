#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

struct channel;

SDL_Point channel_get_position(struct channel *channel);
struct plot *channel_get_plot0(struct channel *channel);
struct plot *channel_get_plot1(struct channel *channel);
bool channel_get_state(struct channel *channel);

struct channel *channel_alloc(void);
void channel_build(struct channel *channel, SDL_Point position,
                   char const *plot0_name, char const *plot1_name,
                   SDL_Rect pos_num);
void channel_free(struct channel *channel);
