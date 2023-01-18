#include "draw.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"
#include "graphics.h"
#include "parse.h"
#include "plot.h"
#include "rchannel.h"
#include "schannel.h"
#include "text.h"

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
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

static inline void draw_red_line_plot(struct plot *plot) {
  float const x0 = plot->fft.x0;
  float const start_x = 0;
  float const mid_y = x0 + (float)plot->position.h / 2;

  float const end_start_x = start_x + plot->position.w;
  float const mid_dy = mid_y + 40;
  SDL_SetRenderDrawColor(renderer, 0xDF, 0x40, 0x53, 0xFF);
  SDL_RenderSetViewport(renderer, &plot->position);
  SDL_RenderDrawLineF(renderer, start_x, mid_dy, end_start_x, mid_dy);
  SDL_RenderSetViewport(renderer, &g_graphics->pos);
}

static inline void draw_plot_data(struct plot *plot) {
  if (check_zero_array(plot->fft.data, plot->fft.length)) {
    draw_red_line_plot(plot);
    return;
  }

  float const dx = plot->fft.dx;
  float const x0 = plot->fft.x0;
  /* float const start_x = plot->position.x; */
  float const start_x = 0;
  /* float const mid_y = (float)plot->position.y + x0 + */
  /*                          (float)plot->position.h / 2; */
  float const mid_y = x0 + (float)plot->position.h / 2;
  SDL_FPoint prev = {.x = start_x, .y = mid_y};
  SDL_FPoint next = {.x = prev.x, .y = prev.y};

  size_t const fft_length = plot->fft.length;
  SDL_RenderSetViewport(renderer, &plot->position);
  SDL_SetRenderDrawColor(renderer, 0x3B, 0x94, 0xE5, 0xFF);
  for (size_t j = 0; j < fft_length; ++j) {
    float const *fft = plot->fft.data;

    next.y = fft[j] + mid_y + plot->scale;
    SDL_RenderDrawLineF(renderer, prev.x, prev.y, next.x, next.y);
    next.x += dx;
    prev.x = next.x;
    prev.y = next.y;
  }
  SDL_RenderSetViewport(renderer, &g_graphics->pos);
}

static inline void draw_plots(void) {
  struct plot **plots = graphics_plots_init(g_graphics);
  size_t idx = 0;
  for (size_t i = 0; i < g_graphics->service->count; ++i) {
    if (!g_graphics->service->schs[i]->state) {
      draw_red_line_plot(plots[idx]);
      draw_red_line_plot(plots[idx + 1]);
      idx += 2;
    } else {
      draw_plot_data(plots[idx]);
      draw_plot_data(plots[idx + 1]);
      idx += 2;
    }
  }

  for (size_t i = 0; i < g_graphics->relay->count; ++i) {
    if (!g_graphics->relay->rchs[i]->state) {
      draw_red_line_plot(plots[idx]);
      draw_red_line_plot(plots[idx + 1]);
      idx += 2;
    } else {
      draw_plot_data(plots[idx]);
      draw_plot_data(plots[idx + 1]);
      idx += 2;
    }
  }
  graphics_plots_free(plots);
}

static inline void draw_fps(void) {
  SDL_Point pos = {.y = 10};
  char buff[100] = {0};
  sprintf(buff, "%d", g_graphics->fps);

  struct text *fps =
      text_init(text_get_font_type(TEXT_FONT_BOLD), 60, COLOR_GREEN, pos, buff);
  fps->position.x = g_graphics->pos.w - fps->position.w - 10;

  DRAW_IN_REN(fps->texture, &fps->position);
  text_free(fps);
}

static inline void draw_schannels(struct vec_schannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {
    struct schannel *sch = vec->schs[i];

    // draw back
    DRAW_IN_REN(sch->plot0->background, &sch->plot0->position);
    DRAW_IN_REN(sch->plot1->background, &sch->plot1->position);

    // draw name
    DRAW_IN_REN(sch->plot0->name->texture, &sch->plot0->name->position);
    DRAW_IN_REN(sch->plot1->name->texture, &sch->plot1->name->position);

    // draw channels number
    DRAW_IN_REN(sch->number->texture, &sch->number->position);
  }
}

static inline void draw_rchannels(struct vec_rchannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {

    struct rchannel *rch = vec->rchs[i];

    // draw back
    DRAW_IN_REN(rch->plot0->background, &rch->plot0->position);
    DRAW_IN_REN(rch->plot1->background, &rch->plot1->position);

    // draw name
    DRAW_IN_REN(rch->plot0->name->texture, &rch->plot0->name->position);
    DRAW_IN_REN(rch->plot1->name->texture, &rch->plot1->name->position);

    // draw channels number
    DRAW_IN_REN(rch->number.texture, &rch->number.position);
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244 * 2, .h = 2, .w = g_graphics->pos.w};
  SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(renderer, &rec);
}

/* static inline void draw_coord_info(struct plot *plot) { */
/*   /\* if(mouse.x  plot->position.x) *\/ */
/* } */

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_schannels(g_graphics->service);
  draw_rchannels(g_graphics->relay);
  draw_line_channel_delim();
  /* draw_fps(); */
  draw_plots();
  SDL_RenderPresent(renderer);
}

void handle_events(void) {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
#endif
  int32_t const frame_delay = 1000 / g_graphics->fps;
  uint32_t frame_start;
  int32_t frame_time;

  frame_start = SDL_GetTicks();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      graphics_free(g_graphics);
      exit(EXIT_SUCCESS);
    } else if (event.type == SDL_MOUSEMOTION) {
      SDL_GetMouseState(&mouse.x, &mouse.y);
    } else if (event.type == SDL_WINDOWEVENT) {
    }
  }

  draw();

  frame_time = SDL_GetTicks() - frame_start;

  if (frame_delay > frame_time) {
    SDL_Delay(frame_delay - frame_time);
  }
}
