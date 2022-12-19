#include "channel.h"
#include "colorscheme.h"
#include "text.h"
#include <SDL2/SDL_pixels.h>

channel_t *Channel_init(SDL_Point position, char const* head_text, char const *channel_number,
                        char const *plot0_name, char const *plot1_name) {
  channel_t *new_channel = malloc(sizeof(*new_channel));
  new_channel->position = position;

  new_channel->head_background =
    (SDL_Rect){ .x = new_channel->position.x, .y = new_channel->position.y,
                .h = 40, .w = 654};

  SDL_Rect pos_head_text = { .x = new_channel->head_background.x, .y = new_channel->head_background.y};
  new_channel->head_text = Text_init(font_type(FONT_R), 12,
                                     COLOR_CHANNEL_HEAD, pos_head_text, head_text);

  SDL_Rect pos_num = {.x = new_channel->head_background.h + 52, .y = position.y + 26};
  new_channel->channel_number = Text_init(font_type(FONT_B), 150, COLOR_CHANNEL_NUMBER,
                                          pos_num, channel_number);

  int32_t const plot_size = 20;
  SDL_Rect pos_plot0 = {.x = pos_num.x + 100, .y = pos_num.y - 20};
  new_channel->plot0_name = Text_init(font_type(FONT_B), plot_size, COLOR_PLOT_NAME,
                                      pos_plot0, plot0_name);

  SDL_Rect pos_plot1 = {.x = pos_plot0.x + 200, .y = pos_plot0.y};
  new_channel->plot1_name = Text_init(font_type(FONT_B), plot_size, COLOR_PLOT_NAME,
                                      pos_plot1, plot1_name);

  return new_channel;
}

void Channel_free(channel_t *channel) {
  if (channel == NULL) {
    return;
  }

  Text_free(channel->head_text);
  Text_free(channel->channel_number);
  Text_free(channel->plot0_name);
  Text_free(channel->plot1_name);

  free(channel);
  channel = NULL;
}
