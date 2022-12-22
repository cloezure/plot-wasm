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

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244*2, .h = 2, .w = g_graphics->width};
  SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(renderer, &rec);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_channels(g_graphics->service_channel);
  draw_line_channel_delim();
  draw_channels(g_graphics->relay_channel);
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