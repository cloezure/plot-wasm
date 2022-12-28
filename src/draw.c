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

  /* plot_t **plots = malloc(sizeof(plot_t*) * 12); */
  /* for(size_t i = 0, j = 0; i < 4; ++i, j += 2) { */
  /*   plots[j] = g_graphics->service_channel->channel[i]->plot0; */
  /*   plots[j+1] = g_graphics->service_channel->channel[i]->plot1; */
  /* } */
  /* for(size_t i = 0, j = 8; i < 2; ++i, j += 2) { */
  /*   plots[j] = g_graphics->relay_channel->channel[i]->plot0; */
  /*   plots[j+1] = g_graphics->relay_channel->channel[i]->plot1; */
  /* } */

  float spc_x = ((channel_service_t*)g_graphics->service_channel->channel[0])->channel->plot0->position.x;
  SDL_SetRenderDrawColor(renderer, 0x3B, 0x94, 0xE5, 0xFF);
  for(size_t i = 0; i < g_plots_count; ++i) {
    for(size_t j = 0; j < g_graphics->fft_size; ++j) {
      SDL_RenderDrawLineF(renderer,
                          j + spc_x,
                          g_graphics->fft[i][j],
                          j + 1,
                          g_graphics->fft[i][j]);
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
