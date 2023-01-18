#include "graphics.h"
#include "channel.h"
#include "channel_relay.h"
#include "channel_service.h"
#include "colorscheme.h"
#include "common_function.h"
#include "coord_info.h"
#include "global.h"
#include "parse.h"
#include "plot.h"
#include "text.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

static inline size_t index_plot_switch(size_t idx);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps) { g_graphics->fps = fps; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void trans_plot_data(int plot_idx, float *data, int length, float dx,
                     float x0) {
  if (plot_idx < 0 || plot_idx > (int)g_plots_count)
    return;

  plot_idx = index_plot_switch(plot_idx);

  struct plot **plots = graphics_plots_cons(g_graphics);
  plot_fft_update(plots[plot_idx], data, length, dx / (100000000 - 25000000),
                  x0 / 10000);
  graphics_plots_free(plots);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *logger(void) {
  static char buffer_logger[100];
  sprintf(buffer_logger, "%s", "Some log..");
  return buffer_logger;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void window_size(int w, int h) { SDL_SetWindowSize(window, w, h); }

struct plot **graphics_plots_cons(struct graphics *graphics) {
  struct plot **plots = malloc(sizeof(struct plot *) * g_plots_count);

  size_t plot_i = 0;
  for (size_t i = 0; i < graphics->service->count; ++i) {
    plots[plot_i] = ((struct channel_service *)graphics->service->channels[i])
                        ->channel->plot0;
    plots[plot_i + 1] =
        ((struct channel_service *)graphics->service->channels[i])
            ->channel->plot1;
    plot_i += 2;
  }

  for (size_t i = 0; i < graphics->relay->count; ++i) {
    plots[plot_i] =
        ((struct channel_relay *)graphics->relay->channels[i])->channel->plot0;
    plots[plot_i + 1] =
        ((struct channel_relay *)graphics->relay->channels[i])->channel->plot1;
    plot_i += 2;
  }

  return plots;
}

void graphics_plots_free(struct plot **plots) { free(plots); }

static inline void off_channel_relay(struct vec_channel *vec, int channel_idx) {
  if (vec->channels[channel_idx]->state == false)
    return;
  vec->channels[channel_idx]->state = false;

  channel_relay_switch_number(
      (struct channel_relay *)vec->channels[channel_idx]);
}

static inline void off_channel_service(struct vec_channel *vec,
                                       int channel_idx) {
  if (vec->channels[channel_idx]->state == false)
    return;
  vec->channels[channel_idx]->state = false;

  struct channel_service *serv =
      (struct channel_service *)vec->channels[channel_idx];

  text_change_color(serv->channel_number, COLOR_CHANNEL_NUMBER_OFF);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void off_channel(int channel_idx) {
  if (channel_idx < 0 || channel_idx >= (int)g_plots_count / 2)
    return;

  int const serv_count = g_graphics->service->count - 1;
  if (channel_idx <= serv_count) {
    off_channel_service(g_graphics->service, channel_idx);
  } else {
    off_channel_relay(g_graphics->relay, channel_idx - 4);
  }
}

struct graphics *graphics_cons(int32_t width, int32_t height, int32_t fps) {
  struct graphics *new_graphics = malloc(sizeof *new_graphics);
  assert(new_graphics);
  new_graphics->pos = (SDL_Rect){.x = 0, .y = 0, .w = width, .h = height};
  new_graphics->width_mid = width / 2;
  new_graphics->height_mid = height / 2;
  new_graphics->fps = fps;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    display_error_sdl("sdl could not init");
    return NULL;
  }

  window =
      SDL_CreateWindow("Odas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       new_graphics->pos.w, new_graphics->pos.h,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (window == NULL) {
    display_error_sdl("window could not be created");
    return NULL;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

  if (renderer == NULL) {
    display_error_sdl("renderer could not be created");
    return NULL;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    puts("Warning: linear texture filtering not enabled!");
  }

  if (TTF_Init() < 0) {
    display_error_ttf("TTF could not init");
    return NULL;
  }

  int32_t flags = IMG_INIT_PNG;
  if (!IMG_Init(flags) && flags) {
    display_error_img("Image could not init");
    return NULL;
  }

  new_graphics->service =
      vec_channel_service_cons(4, (SDL_Point){.x = 0, .y = 0});
  new_graphics->relay =
      vec_channel_relay_cons(2, (SDL_Point){.x = 0, .y = 244 * 2});

  /* new_graphics->coord_dots = NULL; */

  return new_graphics;
}

void graphics_free(struct graphics *graphics) {
  vec_channel_service_free(graphics->service);
  vec_channel_relay_free(graphics->relay);

  free(graphics);
  graphics = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  TTF_Quit();
  SDL_Quit();
}

static inline size_t index_plot_switch(size_t idx) {
  switch (idx) {
  case 0:
    return 1;
  case 1:
    return 3;
  case 2:
    return 5;
  case 3:
    return 7;
  case 4:
    return 0;
  case 5:
    return 2;
  case 6:
    return 4;
  case 7:
    return 6;
  case 8:
    return 9;
  case 9:
    return 8;
  case 10:
    return 11;
  case 11:
    return 10;
  default:
    return 0;
  }
}
