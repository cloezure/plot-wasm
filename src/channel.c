#include "channel.h"
#include "colorscheme.h"
#include "text.h"
#include <SDL2/SDL_pixels.h>

enum FONT_TYPE { FONT_R, FONT_B };

static char const *font_type(enum FONT_TYPE type) {
  switch (type) {
  case FONT_R:
    return "res/Gilroy-Regular.ttf";
  case FONT_B:
    return "res/Gilroy-Bold.ttf";
  }
}

static inline void Channel_init_text(channel_t *channel, SDL_Rect position,
                                     int32_t size, enum FONT_TYPE type,
                                     SDL_Color color, char const *info) {
  channel->channel_number =
      Text_init(font_type(type), size, color, position, info);
}

channel_t *Channel_init(SDL_Rect position, char const *channel_number,
                        char const *plot0_name, char const *plot1_name) {
  channel_t *new_channel = malloc(sizeof(*new_channel));
  new_channel->position = position;

  SDL_Rect pos_num = {.x = position.x + 40, .y = position.y + 40};
  Channel_init_text(new_channel, pos_num, 40, FONT_B, COLOR_CHANNEL_NUMBER,
                    channel_number);

  SDL_Rect pos_plot0 = {.x = pos_num.x + 30, .y = pos_num.y};
  Channel_init_text(new_channel, pos_plot0, 20, FONT_R, COLOR_PLOT_NAME,
                    plot0_name);

  SDL_Rect pos_plot1 = {.x = pos_plot0.x + 50, .y = pos_plot0.y};
  Channel_init_text(new_channel, pos_plot1, 40, FONT_R, COLOR_PLOT_NAME,
                    plot1_name);

  return new_channel;
}

void Channel_free(channel_t *channel) {
  if (channel == NULL) {
    return;
  }

  Text_free(channel->channel_number);
  Text_free(channel->plot0_name);
  Text_free(channel->plot1_name);

  free(channel);
  channel = NULL;
}
