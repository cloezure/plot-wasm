#include "channel.h"
#include "common_function.h"
#include "global.h"
#include "plot.h"
#include "text.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

struct channel {
  SDL_Point position;
  struct plot *plot0;
  struct plot *plot1;
  bool state;
};

SDL_Point channel_get_position(struct channel *channel) {
  return channel->position;
}

struct plot *channel_get_plot0(struct channel *channel) {
  return channel->plot0;
}

struct plot *channel_get_plot1(struct channel *channel) {
  return channel->plot1;
}

bool channel_get_state(struct channel *channel) { return channel->state; }

struct channel *channel_alloc(void) {
  return malloc(sizeof(struct channel));
}

void channel_build(struct channel *channel, SDL_Point position,
                   char const *plot0_name, char const *plot1_name,
                   SDL_Rect pos_num) {
  channel->position = position;
  // plots
  SDL_Point pos_plot0_body = {.x = 69 + 48 + pos_num.x, .y = pos_num.y - 25};
  channel->plot0 = plot_build(pos_plot0_body, plot0_name);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x +
                                   plot_get_pos_w(channel->plot0) + 73,
                              .y = pos_num.y - 25};
  channel->plot1 = plot_build(pos_plot1_body, plot1_name);

  channel->state = true;
}

void channel_free(struct channel *channel) {
  plot_free(channel->plot0);
  plot_free(channel->plot1);
  channel->state = false;

  free(channel);
  channel = NULL;
}
