#include "channel.h"
#include "colorscheme.h"
#include "common_function.h"
#include "plot.h"
#include "text.h"

#include <SDL2/SDL_pixels.h>

channel_t *Channel_init(SDL_Point position, int32_t channel_number,
                        char const *plot0_name, char const *plot1_name) {
  channel_t *new_channel = malloc(sizeof(*new_channel));
  new_channel->position = position;

  char channel_number_s[100];
  sprintf(channel_number_s, "%d", channel_number);

  // body
  SDL_Rect pos_num = {.x = position.x + 52, .y = position.y + 26};
  new_channel->channel_number = Text_init(font_type(FONT_B), 150, COLOR_CHANNEL_NUMBER,
                                          pos_num, channel_number_s);

  int32_t const plot_size = 20;
  SDL_Rect pos_plot0 = {.x = pos_num.x + 100, .y = pos_num.y - 20};
  new_channel->plot0_name = Text_init(font_type(FONT_B), plot_size, COLOR_PLOT_NAME,
                                      pos_plot0, plot0_name);

  SDL_Rect pos_plot1 = {.x = pos_plot0.x + 200, .y = pos_plot0.y};
  new_channel->plot1_name = Text_init(font_type(FONT_B), plot_size, COLOR_PLOT_NAME,
                                      pos_plot1, plot1_name);

  // plots
  SDL_Point pos_plot0_body = {.x = pos_num.x + 100, .y = pos_num.y - (pos_num.h/2)};
  new_channel->plot0 = Plot_init(pos_plot0_body);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x + new_channel->plot0->position.w + 73,
                              .y = pos_num.y - (pos_num.h/2)};
  new_channel->plot1 = Plot_init(pos_plot1_body);

  return new_channel;
}

void Channel_free(channel_t *channel) {
  if (channel == NULL) {
    return;
  }

  Text_free(channel->channel_number);
  Text_free(channel->plot0_name);
  Text_free(channel->plot1_name);

  Plot_free(channel->plot0);
  Plot_free(channel->plot1);

  free(channel);
  channel = NULL;
}

channels_t* Channels_init(int32_t count, SDL_Point position) {
  channels_t *channels = malloc(sizeof(*channels));
  channels->count = count;
  channels->channels = malloc(sizeof(channel_t) * count);

  SDL_Point dpos = position;

  // automatizirovat snosku po chety tam dpos.x -= some...
  __REPEAT__(count) {
    channels->channels[i] = Channel_init(dpos, i + 1, "Tx", "Rx");
  }

  return channels;
}

void Channels_free(channels_t* channels) {
  if(channels == NULL) {
    return;
  }

  __REPEAT__(channels->count) {

  }

}
