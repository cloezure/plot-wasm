#include "channel_service.h"
#include "channel.h"
#include "colorscheme.h"
#include "text.h"

struct channel *channel_service_cons(SDL_Point position, int32_t channel_number,
                                     char const *plot0_name,
                                     char const *plot1_name) {
  struct channel_service *schannel = malloc(sizeof *schannel);
  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};
  schannel->channel = channel_cons(position, plot0_name, plot1_name, pos_num);

  char channel_number_s[10] = {'\0'};
  sprintf(channel_number_s, "%d", channel_number);

  // body
  schannel->channel_number = text_cons(
      text_get_font_type(TEXT_FONT_BOLD), 150, COLOR_CHANNEL_NUMBER_ON,
      (SDL_Point){.x = pos_num.x, .y = pos_num.y}, channel_number_s);

  return (struct channel *)schannel;
}

void channel_service_free(struct channel_service *schannel) {
  if (schannel == NULL) {
    return;
  }

  channel_free(schannel->channel);
  text_free(schannel->channel_number);
  free(schannel);
  schannel = NULL;
}
