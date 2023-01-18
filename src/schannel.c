#include "schannel.h"
#include "colorscheme.h"
#include "comfun.h"
#include "text.h"

struct schannel *schannel_init(SDL_Point position, int32_t channel_number,
                               char const *plot0_name, char const *plot1_name) {
  struct schannel *schannel = malloc(sizeof *schannel);
  SDL_Rect pos_num = {.x = position.x + 26, .y = position.y + 67};

  SDL_Point pos_plot0_body = {.x = 69 + 48 + pos_num.x, .y = pos_num.y - 25};
  schannel->plot0 = plot_init(pos_plot0_body, plot0_name);

  SDL_Point pos_plot1_body = {.x = pos_plot0_body.x +
                                   schannel->plot0->position.w + 73,
                              .y = pos_num.y - 25};
  schannel->plot1 = plot_init(pos_plot1_body, plot1_name);
  schannel->state = true;

  char channel_number_s[50] = {'\0'};
  sprintf(channel_number_s, "%d", channel_number);

  // body
  schannel->number = text_init(
      text_get_font_type(TEXT_FONT_BOLD), 150, COLOR_CHANNEL_NUMBER_ON,
      (SDL_Point){.x = pos_num.x, .y = pos_num.y}, channel_number_s);

  return schannel;
}

void schannel_free(struct schannel *schannel) {
  if (schannel == NULL) {
    return;
  }

  plot_free(schannel->plot0);
  plot_free(schannel->plot1);
  text_free(schannel->number);
  free(schannel);
  schannel = NULL;
}

struct vec_schannel *vec_schannel_init(size_t count, SDL_Point position) {
  struct vec_schannel *vec = malloc(sizeof *vec);
  vec->count = count;
  vec->schs = malloc(sizeof(struct schannel *) * count);

  SDL_Point dpos = position;
  for (size_t i = 0; i < count; ++i) {
    vec->schs[i] = schannel_init(dpos, i + 1, "Tx", "Rx");

    if (!is_even(i)) {
      dpos.y += 244;
      dpos.x -= 654 * 2;
    }

    dpos.x += 654;
  }

  return vec;
}

void vec_schannel_free(struct vec_schannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {
    if (vec->schs[i] == NULL)
      continue;
    schannel_free(vec->schs[i]);
    vec->schs[i] = NULL;
  }

  free(vec->schs);
  vec->schs = NULL;

  free(vec);
  vec = NULL;
}

void off_schannel(struct vec_schannel *vec, int sch_idx) {
  struct schannel *sch = vec->schs[sch_idx];
  if (sch->state == false)
    return;
  sch->state = false;

  text_change_color(sch->number, COLOR_CHANNEL_NUMBER_OFF);
}
