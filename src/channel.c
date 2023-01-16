#include "channel.h"
#include "common_function.h"
#include "global.h"
#include "plot.h"
#include "text.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

struct channel *channel_cons(SDL_Point position, char const *plot0_name,
                             char const *plot1_name, SDL_Rect pos_num) {
  struct channel *channel = malloc(sizeof *channel);
  channel->position = position;
  // plots
  SDL_Point pos_plot0_body = {.x = 69 + 48 + pos_num.x, .y = pos_num.y - 25};
  channel->plot0 = plot_cons(pos_plot0_body, plot0_name);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x +
                                   channel->plot0->position.w + 73,
                              .y = pos_num.y - 25};
  channel->plot1 = plot_cons(pos_plot1_body, plot1_name);
  channel->state = true;

  return channel;
}

void channel_free(struct channel *channel) {
  plot_free(channel->plot0);
  plot_free(channel->plot1);
  free(channel);
  channel = NULL;
}
