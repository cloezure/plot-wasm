#include "channel_service.h"
#include "channel.h"
#include "colorscheme.h"
#include "text.h"

struct channel_service {
  struct channel *channel;
  struct text *channel_number;
};

struct channel_service *channel_service_build(SDL_Point position,
                                              int32_t channel_number,
                                              char const *plot0_name,
                                              char const *plot1_name) {
  struct channel_service *new_channel = malloc(sizeof *new_channel);
  new_channel->channel = channel_alloc();

  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  channel_build(new_channel->channel, position, plot0_name, plot1_name,
                pos_num);

  char channel_number_s[10] = {'\0'};
  sprintf(channel_number_s, "%d", channel_number);

  // body
  new_channel->channel_number =
      text_build(text_get_font_type(TEXT_FONT_BOLD), 150,
                 COLOR_CHANNEL_NUMBER_ON, pos_num, channel_number_s);

  return new_channel;
}

void channel_service_free(struct channel_service *channel) {
  if (channel == NULL) {
    return;
  }

  struct channel_service *schannel = (struct channel_service *)channel;
  channel_free(schannel->channel);

  text_free(schannel->channel_number);
  free(schannel);
  channel = NULL;
}
