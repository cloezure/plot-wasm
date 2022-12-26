#include "draw.h"
#include "graphics.h"
#include "channel.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"
#include "text.h"
#include "parse.h"

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

static inline void draw_background(SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer);
}

static inline void draw_plots(void) {
  char *decode = base64_decode(g_graphics->data_graphics);

  uint32_t spectrums_count = get_uint32(decode, 0);
  uint32_t reports_count = get_uint32(decode, 4);
  int32_t spectrum_config_size = 4 + 4 + (4 * reports_count);
  coordlist coordinate_list = NULL;

  if(flag_draw) {
    puts(decode);
    printf("spect count = %d\nreports count = %d\nspectrum config size = %d\n",
           spectrums_count, reports_count, spectrum_config_size);
    flag_draw = false;
  }

  for(ptrdiff_t spec_idx = 0; spec_idx < spectrums_count; ++spec_idx) {
    int32_t offset = 8 + (spec_idx * spectrum_config_size);
    int32_t dx = get_float(decode, offset);
    int32_t ddx = dx;
    int32_t x0 = get_float(decode, offset + 4);
    /* float* fft_reports = malloc(sizeof(*fft_reports) * spectrum_config_size); */

    /* char buff_fft_reports[100] = {0}; */
    /* buff_fft_reports = slice(decode, &buff_fft_reports, offset + 8, offset + spectrum_config_size); */

    /* __REPEAT__(spectrum_config_size) { */
    /*   fft_reports[i] = atof(buff_fft_reports[i]); */
    /* } */



    /* free(fft_reports); */
  }


  free(decode);
}

static inline void draw_fps(void) {
  SDL_Rect pos = {.y = 10};
  char buff[100] = {0};
  sprintf(buff, "%d", g_graphics->fps);

  text_t * fps = Text_init(font_type(FONT_B), 60, COLOR_GREEN, pos, buff);
  fps->position.x = g_graphics->width - fps->position.w - 10;

  DRAW_IN_REN(fps->texture, &fps->position);
  Text_free(fps);
}

static inline void draw_channels(channels_t *channels) {
  __REPEAT__(channels->count) {

    channel_t *channel = channels->channel[i];
    // draw plots background
    DRAW_IN_REN(channel->plot0->background,
                &channel->plot0->position);

    DRAW_IN_REN(channel->plot1->background,
                &channel->plot1->position);

    // draw channels number
    DRAW_IN_REN(channel->channel_number->texture,
                &channel->channel_number->position);

    // draw plots name
    DRAW_IN_REN(channel->plot0_name->texture,
                &channel->plot0_name->position);
    DRAW_IN_REN(channel->plot1_name->texture,
                &channel->plot1_name->position);
  }
}

static inline void draw_channels_relay(channels_relay_t *channels) {
  __REPEAT__(channels->count) {

    channel_relay_t *channel = channels->channel[i];
    // draw plots background
    DRAW_IN_REN(channel->plot0->background,
                &channel->plot0->position);

    DRAW_IN_REN(channel->plot1->background,
                &channel->plot1->position);

    // draw channels number
    DRAW_IN_REN(channel->channel_number,
                &channel->channel_number_pos);

    // draw plots name
    DRAW_IN_REN(channel->plot0_name->texture,
                &channel->plot0_name->position);
    DRAW_IN_REN(channel->plot1_name->texture,
                &channel->plot1_name->position);
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244*2, .h = 2, .w = g_graphics->width};
  SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(renderer, &rec);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_channels(g_graphics->service_channel);
  draw_line_channel_delim();
  draw_channels_relay(g_graphics->relay_channel);
  draw_fps();
  SDL_RenderPresent(renderer);
}

void handle_events(void) {
  const int32_t frame_delay = 1000 / g_graphics->fps;
  uint32_t frame_start;
  int32_t frame_time;

  frame_start = SDL_GetTicks();

  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT: {
    Graphics_free(g_graphics);
    exit(EXIT_SUCCESS);
    break;
  }
  }
  draw();

  frame_time = SDL_GetTicks() - frame_start;

  if (frame_delay > frame_time) {
    SDL_Delay(frame_delay - frame_time);
  }

}
