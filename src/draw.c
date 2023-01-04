#include "draw.h"
#include "channel.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"
#include "graphics.h"
#include "parse.h"
#include "text.h"

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_pixels.h>
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

  float const dx = g_graphics->plots[0]->fft.dx;
  float const x0 = g_graphics->plots[0]->fft.x0;
  SDL_SetRenderDrawColor(renderer, 0x3B, 0x94, 0xE5, 0xFF);
  for (size_t i = 0; i < g_plots_count; ++i) {
    float start_x = g_graphics->plots[i]->position.x;
    float mid_y_plot = (float)g_graphics->plots[i]->position.y + x0 +
                       (float)g_graphics->plots[i]->position.h / 2;
    SDL_FPoint prev = {.x = start_x, .y = mid_y_plot};
    SDL_FPoint next = {.x = prev.x, .y = prev.y};

    for (size_t j = 0; j < g_graphics->plots[i]->fft.length - 1; ++j) {
      float const *fft = g_graphics->plots[i]->fft.data;

      next.y = fft[j] + mid_y_plot;
      SDL_RenderDrawLineF(renderer, prev.x, prev.y, next.x, next.y);
      next.x += dx;
      prev.x = next.x;
      prev.y = next.y;
    }
  }

  g_graphics_ready = false;
}

inline void draw_fps(void) {
  SDL_Rect pos = {.y = 10};
  char buff[100] = {0};
  sprintf(buff, "%d", g_graphics->fps);

  struct text *fps = text_crealloc(TEXT_FONT_BOLD, 60, COLOR_GREEN, pos, buff);
  fps->position.x = g_graphics->width - fps->position.w - 10;

  _draw_in_ren_(fps->texture, &fps->position);
  text_free(fps);
}

static inline void draw_plots_background(struct channel *channel) {
  // draw plots background
  _draw_in_ren_(channel->plot0->background, &channel->plot0->position);

  _draw_in_ren_(channel->plot1->background, &channel->plot1->position);
}

static inline void draw_plots_name(struct channel *channel) {
  _draw_in_ren_(channel->plot0_name->texture, &channel->plot0_name->position);
  _draw_in_ren_(channel->plot1_name->texture, &channel->plot1_name->position);
}

static inline void draw_channels_service(struct channels *channels) {
  for (size_t i = 0; i < channels->count; ++i) {

    struct channel_service *schannel =
        (struct channel_service *)channels->channels[i];

    draw_plots_background(schannel->channel);

    draw_plots_name(schannel->channel);

    // draw channels number
    _draw_in_ren_(schannel->channel_number->texture,
                  &schannel->channel_number->position);
  }
}

static inline void draw_channels_relay(struct channels *channels) {
  for (size_t i = 0; i < channels->count; ++i) {

    struct channel_relay *rchannel =
        (struct channel_relay *)channels->channels[i];

    // draw plots background
    draw_plots_background(rchannel->channel);

    // draw plots name
    draw_plots_name(rchannel->channel);

    // draw channels number
    _draw_in_ren_(rchannel->channel_number, &rchannel->channel_number_pos);
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244 * 2, .h = 2, .w = g_graphics->width};
  SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(renderer, &rec);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_channels_service(g_graphics->service_channel);
  draw_channels_relay(g_graphics->relay_channel);
  draw_line_channel_delim();
  /* draw_fps(); */
  if (g_graphics_ready) {
    draw_plots();
  }
  SDL_RenderPresent(renderer);
}

void handle_events(void) {
  int32_t const frame_delay = 1000 / g_graphics->fps;
  uint32_t frame_start;
  int32_t frame_time;

  frame_start = SDL_GetTicks();

  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT: {
    graphics_free(g_graphics);
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
