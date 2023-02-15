#include "draw.h"
#include "colorscheme.h"
#include "common_functions.h"
#include "global_vars.h"
#include "graphics.h"
#include "info_win.h"
#include "lang.h"
#include "plot.h"
#include "relay_channel.h"
#include "service_channel.h"
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

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static inline void draw(void);
static inline void draw_background(SDL_Color color);
static inline void draw_plot_data(struct plot *plot);
static inline void draw_plots(void);
static inline void draw_fps(void);
static inline void draw_rchannels(struct vec_rchannel *vec);
static inline void draw_schannels(struct vec_schannel *vec);
static inline void draw_line_channel_delim(void);
static inline void draw_info_win16(struct info_win16 *info_win);
static inline void draw_red_line_plot(struct plot *plot, char16_t const *info);
static inline void draw_in_plot_info(struct plot *plot, char16_t const *info);

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
  float const mx = g_graphics->mouse_position.x;
  float const my = g_graphics->mouse_position.y;
  float const ph = plot->position.h + plot->position.y;
  float const pw = plot->position.w + plot->position.x;

  if (mx >= plot->position.x && my >= plot->position.y && mx < pw && my < ph) {
    struct info_win16 *coinf = info_win16_init_text((SDL_Point){mx, my}, info);
    draw_info_win16(coinf);
    info_win16_free(coinf);
    check_click(plot);
  }
}

static inline void draw_red_line_plot(struct plot *plot, char16_t const *info) {
  SDL_Rect const plot_pos = plot->position;
  float const x0 = plot->fft.x0;
  float const start_x = 3;
  float const mid_y = x0 + (float)plot->position.h / 2;

  float const end_start_x = start_x + plot->position.w;
  float const mid_dy = mid_y + 35;
  SDL_SetRenderDrawColor(g_renderer, 0xDF, 0x40, 0x53, 0xFF);
  SDL_RenderSetViewport(g_renderer, &plot_pos);
  SDL_RenderDrawLineF(g_renderer, start_x, mid_dy, end_start_x, mid_dy);

  draw_in_plot_info(plot, info);

  SDL_RenderSetViewport(g_renderer, &g_graphics->position);
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
  float const mid_y = (float)plot->position.h / 2;
  /* SDL_FPoint prev = {.x = plot->fft.data[0].x, */
  /*                    .y = plot->fft.data[0].y + mid_y}; */
  SDL_FPoint prev = {.x = 0, .y = plot->fft.data[0].y + mid_y};
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

  SDL_RenderSetViewport(g_renderer, &g_graphics->position);
}

static inline void draw_plots(void) {
  struct plot **plots = graphics_plots_init(g_graphics);
  size_t idx = 0;
  for (size_t i = 0; i < g_graphics->vec_service->count; ++i) {
    if (!g_graphics->vec_service->channels[i]->state) {
      draw_red_line_plot(plots[idx], g_lang->info[CHANNEL_OFF]);
      draw_red_line_plot(plots[idx + 1], g_lang->info[CHANNEL_OFF]);
      idx += 2;
    } else {
      draw_plot_data(plots[idx]);
      draw_plot_data(plots[idx + 1]);
      idx += 2;
    }
  }

  for (size_t i = 0; i < g_graphics->vec_relay->count; ++i) {
    if (!g_graphics->vec_relay->channels[i]->state) {
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
  fps->position.x = g_graphics->position.w - fps->position.w - 10;

  DRAW_TEXTURE(fps->texture, &fps->position);
  text8_free(fps);
}

static inline void draw_schannels(struct vec_schannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {
    struct schannel *sch = vec->channels[i];

    // draw back
    DRAW_TEXTURE(sch->plot0->background, &sch->plot0->position);
    DRAW_TEXTURE(sch->plot1->background, &sch->plot1->position);

    // draw name
    DRAW_TEXTURE(sch->plot0->name->texture, &sch->plot0->name->position);
    DRAW_TEXTURE(sch->plot1->name->texture, &sch->plot1->name->position);

    // draw channels number
    DRAW_TEXTURE(sch->number->texture, &sch->number->position);

    // draw plot0 hcharts
    DRAW_TEXTURE(sch->plot0->hcharts->unit->texture,
                 &sch->plot0->hcharts->unit->position);

    for (size_t i = 0; i < sch->plot0->hcharts->length; ++i) {
      DRAW_TEXTURE(sch->plot0->hcharts->points[i]->texture,
                   &sch->plot0->hcharts->points[i]->position);
    }

    // draw plot1 hcharts
    DRAW_TEXTURE(sch->plot1->hcharts->unit->texture,
                 &sch->plot1->hcharts->unit->position);

    for (size_t i = 0; i < sch->plot1->hcharts->length; ++i) {
      DRAW_TEXTURE(sch->plot1->hcharts->points[i]->texture,
                   &sch->plot1->hcharts->points[i]->position);
    }

    // draw plot0 vcharts
    DRAW_TEXTURE(sch->plot0->vcharts->unit->texture,
                 &sch->plot0->vcharts->unit->position);

    for (size_t i = 0; i < sch->plot0->vcharts->length; ++i) {
      DRAW_TEXTURE(sch->plot0->vcharts->points[i]->texture,
                   &sch->plot0->vcharts->points[i]->position);
    }

    // draw plot1 vcharts
    DRAW_TEXTURE(sch->plot1->vcharts->unit->texture,
                 &sch->plot1->vcharts->unit->position);

    for (size_t i = 0; i < sch->plot1->vcharts->length; ++i) {
      DRAW_TEXTURE(sch->plot1->vcharts->points[i]->texture,
                   &sch->plot1->vcharts->points[i]->position);
    }
  }
}

static inline void draw_rchannels(struct vec_rchannel *vec) {
  for (size_t i = 0; i < vec->count; ++i) {

    struct rchannel *rch = vec->channels[i];

    // draw back
    DRAW_TEXTURE(rch->plot0->background, &rch->plot0->position);
    DRAW_TEXTURE(rch->plot1->background, &rch->plot1->position);

    // draw name
    DRAW_TEXTURE(rch->plot0->name->texture, &rch->plot0->name->position);
    DRAW_TEXTURE(rch->plot1->name->texture, &rch->plot1->name->position);

    // draw channels number
    DRAW_TEXTURE(rch->number.texture, &rch->number.position);

    // draw plot0 hcharts
    DRAW_TEXTURE(rch->plot0->hcharts->unit->texture,
                 &rch->plot0->hcharts->unit->position);

    for (size_t i = 0; i < rch->plot0->hcharts->length; ++i) {
      DRAW_TEXTURE(rch->plot0->hcharts->points[i]->texture,
                   &rch->plot0->hcharts->points[i]->position);
    }

    // draw plot1 hcharts
    DRAW_TEXTURE(rch->plot1->hcharts->unit->texture,
                 &rch->plot1->hcharts->unit->position);

    for (size_t i = 0; i < rch->plot1->hcharts->length; ++i) {
      DRAW_TEXTURE(rch->plot1->hcharts->points[i]->texture,
                   &rch->plot1->hcharts->points[i]->position);
    }

    // draw plot0 vcharts
    DRAW_TEXTURE(rch->plot0->vcharts->unit->texture,
                 &rch->plot0->vcharts->unit->position);

    for (size_t i = 0; i < rch->plot0->vcharts->length; ++i) {
      DRAW_TEXTURE(rch->plot0->vcharts->points[i]->texture,
                   &rch->plot0->vcharts->points[i]->position);
    }

    // draw plot1 vcharts
    DRAW_TEXTURE(rch->plot1->vcharts->unit->texture,
                 &rch->plot1->vcharts->unit->position);

    for (size_t i = 0; i < rch->plot1->vcharts->length; ++i) {
      DRAW_TEXTURE(rch->plot1->vcharts->points[i]->texture,
                   &rch->plot1->vcharts->points[i]->position);
    }
  }
}

static inline void draw_line_channel_delim(void) {
  SDL_Rect rec = {.x = 0, .y = 244 * 2, .h = 2, .w = g_graphics->position.w};
  SDL_SetRenderDrawColor(g_renderer, 0x1A, 0x1A, 0x1A, 0xFF);
  SDL_RenderDrawRect(g_renderer, &rec);
}

static inline void draw_info_win16(struct info_win16 *info_win) {
  SDL_RenderSetViewport(g_renderer, &g_graphics->position);
  SDL_SetRenderDrawColor(g_renderer, info_win->background.r,
                         info_win->background.g, info_win->background.b,
                         info_win->background.a);
  if ((g_graphics->mouse_position.x + info_win->text->position.w) >
      (g_graphics->position.w - 10)) {
    info_win->text->position.x -= info_win->text->position.w;
  }

  // draw back
  SDL_RenderFillRect(g_renderer, &info_win->text->position);
  // draw text
  DRAW_TEXTURE(info_win->text->texture, &info_win->text->position);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_schannels(g_graphics->vec_service);
  draw_rchannels(g_graphics->vec_relay);
  draw_line_channel_delim();
  /* draw_fps(); */
  draw_plots();
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
      SDL_GetMouseState(&g_graphics->mouse_position.x,
                        &g_graphics->mouse_position.y);
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
