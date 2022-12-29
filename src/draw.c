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

  /* plot_t **plots = malloc(sizeof(plot_t*) * 12); */
  /* for(size_t i = 0, j = 0; i < 4; ++i, j += 2) { */
  /*   plots[j] = g_graphics->service_channel->channel[i]->plot0; */
  /*   plots[j+1] = g_graphics->service_channel->channel[i]->plot1; */
  /* } */
  /* for(size_t i = 0, j = 8; i < 2; ++i, j += 2) { */
  /*   plots[j] = g_graphics->relay_channel->channel[i]->plot0; */
  /*   plots[j+1] = g_graphics->relay_channel->channel[i]->plot1; */
  /* } */

  int w_back = g_graphics->plots[0]->position.w;
  int h_back = g_graphics->plots[0]->position.h;
  float dx = g_graphics->plots[0]->dx;
  float const x0 = g_graphics->plots[0]->x0;
  SDL_SetRenderDrawColor(renderer, 0x3B, 0x94, 0xE5, 0xFF);
  for(size_t i = 0; i < g_plots_count; ++i) {
    float start_x = g_graphics->plots[i]->position.x;
    float mid_y_plot = (float)g_graphics->plots[i]->position.y + x0 + (float)g_graphics->plots[i]->position.h/2;
    SDL_FPoint p = {.x = start_x, .y = mid_y_plot};
    SDL_FPoint n = {.x = p.x, .y = p.y};

    for(size_t j = 0; j < g_graphics->plots[i]->fft_len - 1; ++j) {
      float const* fft = g_graphics->plots[i]->fft;
      /* SDL_RenderDrawLineF(renderer, p.x, p.y, p.x + dx, p.y - fft[j+1]); */
      /* p.x += dx; */
      /* p.y -= fft[j]; */

      n.y = fft[j] + mid_y_plot;
      SDL_RenderDrawLineF(renderer, p.x, p.y, n.x, n.y);
      n.x += dx;
      p.x = n.x;
      p.y = n.y;

      /* SDL_RenderDrawLineF(renderer, */
      /*                     p.x, */
      /*                     p.y, */
      /*                     n.x, */
      /*                     n.y */
      /* ); */
      /* n.y = fft[j] + 70 + mid_y_plot; */
      /* n.x += dx; */
    }
  }

  g_graphics_ready = false;
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

static inline void draw_channels_service(channels_t *channels) {
  __REPEAT__(channels->count) {

    channel_service_t *channel = (channel_service_t*)channels->channel[i];
    // draw plots background
    DRAW_IN_REN(channel->channel->plot0->background,
                &channel->channel->plot0->position);

    DRAW_IN_REN(channel->channel->plot1->background,
                &channel->channel->plot1->position);

    // draw channels number
    DRAW_IN_REN(channel->channel_number->texture,
                &channel->channel_number->position);

    // draw plots name
    DRAW_IN_REN(channel->channel->plot0_name->texture,
                &channel->channel->plot0_name->position);
    DRAW_IN_REN(channel->channel->plot1_name->texture,
                &channel->channel->plot1_name->position);
  }
}

static inline void draw_channels_relay(channels_t *channels) {
  __REPEAT__(channels->count) {

    channel_relay_t *channel = (channel_relay_t*)channels->channel[i];
    // draw plots background
    DRAW_IN_REN(channel->channel->plot0->background,
                &channel->channel->plot0->position);

    DRAW_IN_REN(channel->channel->plot1->background,
                &channel->channel->plot1->position);

    // draw channels number
    DRAW_IN_REN(channel->channel_number,
                &channel->channel_number_pos);

    // draw plots name
    DRAW_IN_REN(channel->channel->plot0_name->texture,
                &channel->channel->plot0_name->position);
    DRAW_IN_REN(channel->channel->plot1_name->texture,
                &channel->channel->plot1_name->position);
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244*2, .h = 2, .w = g_graphics->width};
  SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(renderer, &rec);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_channels_service(g_graphics->service_channel);
  draw_channels_relay(g_graphics->relay_channel);
  draw_line_channel_delim();
  draw_channels_relay(g_graphics->relay_channel);
  draw_fps();
  if(g_graphics_ready) { draw_plots(); }
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
