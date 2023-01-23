#include "draw.h"
#include "coinf.h"
#include "colorscheme.h"
#include "comfun.h"
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
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

static inline void draw(void);
static inline void draw_background(SDL_Color color);
static inline void draw_red_line_plot(struct plot *plot, char const *info,
                                      SDL_Point dpos);
static inline void draw_plot_data(struct plot *plot, SDL_Point dpos);
static inline void draw_plots(void);
static inline void draw_fps(void);
static inline void draw_rchannels(struct vec_rchannel *vec);
static inline void draw_schannels(struct vec_schannel *vec);
static inline void draw_line_channel_delim(void);
static inline void draw_coinf(struct coinf *coinf);

static inline void draw_background(SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer);
}

static inline void draw_red_line_plot(struct plot *plot, char const *info,
                                      SDL_Point dpos) {
  SDL_Rect plot_dpos = (SDL_Rect){.x = dpos.x + plot->position.x,
                                  .y = dpos.y + plot->position.y,
                                  .w = plot->position.w,
                                  .h = plot->position.h};
  float const x0 = plot->fft.x0;
  float const start_x = 0;
  float const mid_y = x0 + (float)plot->position.h / 2;

  float const end_start_x = start_x + plot->position.w;
  float const mid_dy = mid_y + 40;
  SDL_SetRenderDrawColor(renderer, 0xDF, 0x40, 0x53, 0xFF);
  SDL_RenderSetViewport(renderer, &plot_dpos);
  SDL_RenderDrawLineF(renderer, start_x, mid_dy, end_start_x, mid_dy);

  float const mx = g_graphics->mouse.x;
  float const my = g_graphics->mouse.y;
  float const ph = plot->position.h + plot_dpos.y;
  float const pw = plot->position.w + plot_dpos.x;

  if (mx >= plot_dpos.x && my >= plot_dpos.y && mx < pw && my < ph) {
    struct coinf *coinf = coinf_init_text((SDL_Point){mx, my}, info);
    draw_coinf(coinf);
    coinf_free(coinf);
  }

  SDL_RenderSetViewport(renderer, &g_graphics->pos);
}

static inline void draw_plot_data(struct plot *plot, SDL_Point dpos) {
  if (check_zero_array(plot->fft.data, plot->fft.length)) {
    draw_red_line_plot(plot, "no data", dpos);
    return;
  }

  float const dx = fabs(plot->fft.dx);
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
    SDL_FPoint const *fft = plot->fft.data;
    plot->fft.data->x = prev.x;

    next.y = fft[j].y + mid_y;
    SDL_RenderDrawLineF(renderer, prev.x, prev.y, next.x, next.y);
    next.x += dx;
    prev.x = next.x;
    prev.y = next.y;

    float const mx = g_graphics->mouse.x;
    float const my = g_graphics->mouse.y;
    float const ph = plot->position.h + plot->position.y;
    float const pw = plot->position.w + plot->position.x;

    if (mx >= plot->position.x && my >= plot->position.y && mx < pw &&
        my < ph) {
      if (mx == prev.x && my == prev.y) {
        struct coinf *coinf = coinf_init_coord(
            (SDL_Point){mx, my}, plot->fft.data->x, plot->fft.data->y);
        draw_coinf(coinf);
        coinf_free(coinf);
      }
    }
  }

  SDL_RenderSetViewport(renderer, &g_graphics->pos);
}

static inline void draw_plots(void) {
  struct plot **plots = graphics_plots_init(g_graphics);
  size_t idx = 0;
  for (size_t i = 0; i < g_graphics->service->count; ++i) {
    if (!g_graphics->service->schs[i]->state) {
      draw_red_line_plot(plots[idx], "channel off",
                         g_graphics->service->schs[i]->dpos);
      draw_red_line_plot(plots[idx + 1], "channel off",
                         g_graphics->service->schs[i]->dpos);
      idx += 2;
    } else {
      draw_plot_data(plots[idx], g_graphics->service->schs[i]->dpos);
      draw_plot_data(plots[idx + 1], g_graphics->service->schs[i]->dpos);
      idx += 2;
    }
  }

  for (size_t i = 0; i < g_graphics->relay->count; ++i) {
    if (!g_graphics->relay->rchs[i]->state) {
      draw_red_line_plot(plots[idx], "channel off",
                         g_graphics->relay->rchs[i]->dpos);
      draw_red_line_plot(plots[idx + 1], "channel off",
                         g_graphics->relay->rchs[i]->dpos);
      idx += 2;
    } else {
      draw_plot_data(plots[idx], g_graphics->relay->rchs[i]->dpos);
      draw_plot_data(plots[idx + 1], g_graphics->relay->rchs[i]->dpos);
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
    SDL_Rect pos_plot0 = sch->plot0->position;
    SDL_Rect dpos_plot0 = (SDL_Rect){.x = pos_plot0.x + sch->dpos.x,
                                     .y = pos_plot0.y + sch->dpos.y,
                                     .w = pos_plot0.w,
                                     .h = pos_plot0.h};

    DRAW_IN_REN(sch->plot0->background, &dpos_plot0);

    SDL_Rect pos_plot1 = sch->plot1->position;
    SDL_Rect dpos_plot1 = (SDL_Rect){.x = pos_plot1.x + sch->dpos.x,
                                     .y = pos_plot1.y + sch->dpos.y,
                                     .w = pos_plot1.w,
                                     .h = pos_plot1.h};
    DRAW_IN_REN(sch->plot1->background, &dpos_plot1);

    // draw name
    SDL_Rect pos_plot0_name = sch->plot0->name->position;
    SDL_Rect dpos_plot0_name = (SDL_Rect){.x = pos_plot0_name.x + sch->dpos.x,
                                          .y = pos_plot0_name.y + sch->dpos.y,
                                          .w = pos_plot0_name.w,
                                          .h = pos_plot0_name.h};
    DRAW_IN_REN(sch->plot0->name->texture, &dpos_plot0_name);

    SDL_Rect pos_plot1_name = sch->plot1->name->position;
    SDL_Rect dpos_plot1_name = (SDL_Rect){.x = pos_plot1_name.x + sch->dpos.x,
                                          .y = pos_plot1_name.y + sch->dpos.y,
                                          .w = pos_plot1_name.w,
                                          .h = pos_plot1_name.h};
    DRAW_IN_REN(sch->plot1->name->texture, &dpos_plot1_name);

    // draw channels number
    SDL_Rect pos_num = sch->number->position;
    SDL_Rect dpos_num = (SDL_Rect){.x = pos_num.x + sch->dpos.x,
                                   .y = pos_num.y + sch->dpos.y,
                                   .w = pos_num.w,
                                   .h = pos_num.h};
    DRAW_IN_REN(sch->number->texture, &dpos_num);
  }
}

static inline void draw_rchannels(struct vec_rchannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {

    struct rchannel *rch = vec->rchs[i];

    // draw back
    SDL_Rect pos_plot0 = rch->plot0->position;
    SDL_Rect dpos_plot0 = (SDL_Rect){.x = pos_plot0.x + rch->dpos.x,
                                     .y = pos_plot0.y + rch->dpos.y,
                                     .w = pos_plot0.w,
                                     .h = pos_plot0.h};
    DRAW_IN_REN(rch->plot0->background, &dpos_plot0);

    SDL_Rect pos_plot1 = rch->plot1->position;
    SDL_Rect dpos_plot1 = (SDL_Rect){.x = pos_plot1.x + rch->dpos.x,
                                     .y = pos_plot1.y + rch->dpos.y,
                                     .w = pos_plot1.w,
                                     .h = pos_plot1.h};
    DRAW_IN_REN(rch->plot1->background, &dpos_plot1);

    // draw name
    SDL_Rect pos_plot0_name = rch->plot0->name->position;
    SDL_Rect dpos_plot0_name = (SDL_Rect){.x = pos_plot0_name.x + rch->dpos.x,
                                          .y = pos_plot0_name.y + rch->dpos.y,
                                          .w = pos_plot0_name.w,
                                          .h = pos_plot0_name.h};
    DRAW_IN_REN(rch->plot0->name->texture, &dpos_plot0_name);

    SDL_Rect pos_plot1_name = rch->plot1->name->position;
    SDL_Rect dpos_plot1_name = (SDL_Rect){.x = pos_plot1_name.x + rch->dpos.x,
                                          .y = pos_plot1_name.y + rch->dpos.y,
                                          .w = pos_plot1_name.w,
                                          .h = pos_plot1_name.h};
    DRAW_IN_REN(rch->plot1->name->texture, &dpos_plot1_name);

    // draw channels number
    SDL_Rect pos_num = rch->number.position;
    SDL_Rect dpos_num = (SDL_Rect){.x = pos_num.x + rch->dpos.x,
                                   .y = pos_num.y + rch->dpos.y,
                                   .w = pos_num.w,
                                   .h = pos_num.h};
    DRAW_IN_REN(rch->number.texture, &dpos_num);
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244 * 2, .h = 2, .w = g_graphics->pos.w};
  SDL_SetRenderDrawColor(renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(renderer, &rec);
}

static inline void draw_coinf(struct coinf *coinf) {
  SDL_RenderSetViewport(renderer, &g_graphics->pos);
  SDL_SetRenderDrawColor(renderer, coinf->back.r, coinf->back.g, coinf->back.b,
                         coinf->back.a);
  // draw back
  SDL_RenderFillRect(renderer, &coinf->inf_txt->position);
  // draw text
  DRAW_IN_REN(coinf->inf_txt->texture, &coinf->inf_txt->position);
}

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
      SDL_GetMouseState(&g_graphics->mouse.x, &g_graphics->mouse.y);
    }
  }

  draw();

  frame_time = SDL_GetTicks() - frame_start;

  if (frame_delay > frame_time) {
    SDL_Delay(frame_delay - frame_time);
  }
}
