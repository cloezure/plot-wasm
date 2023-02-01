#include "draw.h"
#include "coinf.h"
#include "colorscheme.h"
#include "comfun.h"
#include "global.h"
#include "graphics.h"
#include "lang.h"
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
static inline void draw_plot_data(struct plot *plot);
static inline void draw_plots(void);
static inline void draw_fps(void);
static inline void draw_rchannels(struct vec_rchannel *vec);
static inline void draw_schannels(struct vec_schannel *vec);
static inline void draw_line_channel_delim(void);
static inline void draw_coinf16(struct coinf16 *coinf);
static inline void draw_red_line_plot(struct plot *plot, char16_t const *info);
static inline void draw_in_plot_info(struct plot *plot, char16_t const *info);

SDL_Event event;
bool click_left_mouse = false;

static inline void check_click(struct plot *plot) {
  if (click_left_mouse) {
    g_last_press = plot->index;
    click_left_mouse = false;
  }
}

static inline void draw_background(SDL_Color color) {
  SDL_SetRenderDrawColor(g_renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(g_renderer);
}

static inline void draw_in_plot_info(struct plot *plot, char16_t const *info) {
  float const mx = g_graphics->mouse.x;
  float const my = g_graphics->mouse.y;
  float const ph = plot->position.h + plot->position.y;
  float const pw = plot->position.w + plot->position.x;

  if (mx >= plot->position.x && my >= plot->position.y && mx < pw && my < ph) {
    struct coinf16 *coinf = coinf16_init_text((SDL_Point){mx, my}, info);
    draw_coinf16(coinf);
    coinf16_free(coinf);
    check_click(plot);
  }
}

static inline void draw_red_line_plot(struct plot *plot, char16_t const *info) {
  SDL_Rect const plot_pos = plot->position;
  float const x0 = plot->fft.x0;
  float const start_x = 0;
  float const mid_y = x0 + (float)plot->position.h / 2;

  float const end_start_x = start_x + plot->position.w;
  float const mid_dy = mid_y + 40;
  SDL_SetRenderDrawColor(g_renderer, 0xDF, 0x40, 0x53, 0xFF);
  SDL_RenderSetViewport(g_renderer, &plot_pos);
  SDL_RenderDrawLineF(g_renderer, start_x, mid_dy, end_start_x, mid_dy);

  draw_in_plot_info(plot, info);

  SDL_RenderSetViewport(g_renderer, &g_graphics->pos);
}

/* static void draw_info_coinf_in_rect(struct plot const *plot) { */

/*   float const mx = g_graphics->mouse.x; */
/*   float const my = g_graphics->mouse.y; */
/*   float const ph = plot->position.h + plot->position.y; */
/*   float const pw = plot->position.w + plot->position.x; */

/*   if (mx >= plot->position.x && my >= plot->position.y && mx < pw && my < ph)
 * { */
/*     float const imx = mx - plot->position.x; */
/*     float const imy = my - plot->position.y; */
/*     size_t const len = plot->fft.length; */
/*     float const mid_y = plot->fft.x0 + (float)plot->position.h / 2; */
/*     SDL_FPoint prev = {.x = 0, .y = 0}; */

/*     for (size_t i = 0; i < len; ++i) { */
/*       SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); */
/*       SDL_RenderDrawLineF(renderer, prev.x, */
/*                           plot->fft.data[len - 1 - i].y + */
/*                               ((float)plot->position.h / 2), */
/*                           imx, imy); */
/*       /\* SDL_RenderDrawLine(renderer, prev.x, prev.y, mx, my); *\/ */
/*       prev.x += plot->fft.dx; */
/*       prev.y = plot->fft.data[len - 1 - i].y; */
/*     } */

/*     /\* printf("mx = %f my = %f dd.x = %f dd.y = %f\n", mx, my, dd[i].x, */
/*      * dd[i].y); *\/ */
/*     struct coinf *coinf = coinf_init_coord((SDL_Point){mx, my}, imx, imy); */
/*     draw_coinf(coinf); */
/*     coinf_free(coinf); */
/*   } */
/* } */

static inline void draw_plot_data(struct plot *plot) {
  if (check_zero_array(plot->fft.data, plot->fft.length)) {
    draw_red_line_plot(plot, g_lang->info[NO_DATA]);
    return;
  }

  float const dx = fabs(plot->fft.dx);
  float const x0 = plot->fft.x0;
  /* float const start_x = plot->position.x; */
  float const start_x = 0;
  float const mid_y = x0 + (float)plot->position.h / 2;
  SDL_FPoint prev = {.x = start_x, .y = mid_y};
  SDL_FPoint next = {.x = prev.x, .y = prev.y};

  size_t const fft_length = plot->fft.length;
  SDL_RenderSetViewport(g_renderer, &plot->position);
  SDL_SetRenderDrawColor(g_renderer, 0x3B, 0x94, 0xE5, 0xFF);
  for (size_t j = 0; j < fft_length; ++j) {
    SDL_FPoint const *fft = plot->fft.data;
    plot->fft.data->x = prev.x;

    next.y = fft[j].y + mid_y;
    SDL_RenderDrawLineF(g_renderer, prev.x, prev.y, next.x, next.y);
    next.x += dx;
    prev.x = next.x;
    prev.y = next.y;
  }

  draw_in_plot_info(plot, g_lang->info[CLICK_TO_OPEN]);

  SDL_RenderSetViewport(g_renderer, &g_graphics->pos);
}

static inline void draw_plots(void) {
  struct plot **plots = graphics_plots_init(g_graphics);
  size_t idx = 0;
  for (size_t i = 0; i < g_graphics->service->count; ++i) {
    if (!g_graphics->service->schs[i]->state) {
      draw_red_line_plot(plots[idx], g_lang->info[CHANNEL_OFF]);
      draw_red_line_plot(plots[idx + 1], g_lang->info[CHANNEL_OFF]);
      idx += 2;
    } else {
      draw_plot_data(plots[idx]);
      draw_plot_data(plots[idx + 1]);
      idx += 2;
    }
  }

  for (size_t i = 0; i < g_graphics->relay->count; ++i) {
    if (!g_graphics->relay->rchs[i]->state) {
      draw_red_line_plot(plots[idx], g_lang->info[CHANNEL_OFF]);
      draw_red_line_plot(plots[idx + 1], g_lang->info[CHANNEL_OFF]);
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

  struct text8 *fps = text8_init(text_get_font_type(TEXT_FONT_BOLD), 60,
                                 COLOR_GREEN, pos, buff);
  fps->position.x = g_graphics->pos.w - fps->position.w - 10;

  DRAW_IN_REN(fps->texture, &fps->position);
  text8_free(fps);
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

    // draw plot0 hcharts
    DRAW_IN_REN(sch->plot0->hcharts->unit->texture,
                &sch->plot0->hcharts->unit->position);

    for (size_t i = 0; i < sch->plot0->hcharts->len; ++i) {
      DRAW_IN_REN(sch->plot0->hcharts->points[i]->texture,
                  &sch->plot0->hcharts->points[i]->position);
    }

    // draw plot1 hcharts
    DRAW_IN_REN(sch->plot1->hcharts->unit->texture,
                &sch->plot1->hcharts->unit->position);

    for (size_t i = 0; i < sch->plot1->hcharts->len; ++i) {
      DRAW_IN_REN(sch->plot1->hcharts->points[i]->texture,
                  &sch->plot1->hcharts->points[i]->position);
    }

    // draw plot0 vcharts
    DRAW_IN_REN(sch->plot0->vcharts->unit->texture,
                &sch->plot0->vcharts->unit->position);

    for (size_t i = 0; i < sch->plot0->vcharts->len; ++i) {
      DRAW_IN_REN(sch->plot0->vcharts->points[i]->texture,
                  &sch->plot0->vcharts->points[i]->position);
    }

    // draw plot1 vcharts
    DRAW_IN_REN(sch->plot1->vcharts->unit->texture,
                &sch->plot1->vcharts->unit->position);

    for (size_t i = 0; i < sch->plot1->vcharts->len; ++i) {
      DRAW_IN_REN(sch->plot1->vcharts->points[i]->texture,
                  &sch->plot1->vcharts->points[i]->position);
    }
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

    // draw plot0 hcharts
    DRAW_IN_REN(rch->plot0->hcharts->unit->texture,
                &rch->plot0->hcharts->unit->position);

    for (size_t i = 0; i < rch->plot0->hcharts->len; ++i) {
      DRAW_IN_REN(rch->plot0->hcharts->points[i]->texture,
                  &rch->plot0->hcharts->points[i]->position);
    }

    // draw plot1 hcharts
    DRAW_IN_REN(rch->plot1->hcharts->unit->texture,
                &rch->plot1->hcharts->unit->position);

    for (size_t i = 0; i < rch->plot1->hcharts->len; ++i) {
      DRAW_IN_REN(rch->plot1->hcharts->points[i]->texture,
                  &rch->plot1->hcharts->points[i]->position);
    }

    // draw plot0 vcharts
    DRAW_IN_REN(rch->plot0->vcharts->unit->texture,
                &rch->plot0->vcharts->unit->position);

    for (size_t i = 0; i < rch->plot0->vcharts->len; ++i) {
      DRAW_IN_REN(rch->plot0->vcharts->points[i]->texture,
                  &rch->plot0->vcharts->points[i]->position);
    }

    // draw plot1 vcharts
    DRAW_IN_REN(rch->plot1->vcharts->unit->texture,
                &rch->plot1->vcharts->unit->position);

    for (size_t i = 0; i < rch->plot1->vcharts->len; ++i) {
      DRAW_IN_REN(rch->plot1->vcharts->points[i]->texture,
                  &rch->plot1->vcharts->points[i]->position);
    }
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244 * 2, .h = 2, .w = g_graphics->pos.w};
  SDL_SetRenderDrawColor(g_renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(g_renderer, &rec);
}

static inline void draw_coinf16(struct coinf16 *coinf) {
  SDL_RenderSetViewport(g_renderer, &g_graphics->pos);
  SDL_SetRenderDrawColor(g_renderer, coinf->back.r, coinf->back.g,
                         coinf->back.b, coinf->back.a);
  if ((g_graphics->mouse.x + coinf->inf_txt->position.w) >
      (g_graphics->pos.w - 10)) {
    coinf->inf_txt->position.x -= coinf->inf_txt->position.w;
  }

  // draw back
  SDL_RenderFillRect(g_renderer, &coinf->inf_txt->position);
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

  /* char buff[100] = {'\0'}; */
  /* sprintf(buff, "%d", g_last_press); */
  /* struct text8 *t = text8_init(text_get_font_type(TEXT_FONT_REGULAR), 40, */
  /*                              COLOR_GREEN, MIDDLE_POINT, buff); */

  /* DRAW_IN_REN(t->texture, &t->position); */
  /* text8_free(t); */
}

void handle_events(void) {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
#endif
  int32_t const frame_delay = 1000 / g_graphics->fps;
  uint32_t frame_start;
  int32_t frame_time;

  frame_start = SDL_GetTicks();

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      graphics_free(g_graphics);
      exit(EXIT_SUCCESS);
    } else if (event.type == SDL_MOUSEMOTION) {
      SDL_GetMouseState(&g_graphics->mouse.x, &g_graphics->mouse.y);
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        click_left_mouse = true;
      }
    }
  }

  draw();
  SDL_RenderPresent(g_renderer);

  frame_time = SDL_GetTicks() - frame_start;

  if (frame_delay > frame_time) {
    SDL_Delay(frame_delay - frame_time);
  }
}
