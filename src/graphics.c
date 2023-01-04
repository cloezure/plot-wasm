#include "graphics.h"
#include "channel.h"
#include "common_function.h"
#include "global.h"
#include "parse.h"
#include "plot.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps) { g_graphics->fps = fps; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(int plot_idx, float *data, int length, float dx, float x0) {
  g_graphics->plots[plot_idx]->fft.data = data;
  g_graphics->plots[plot_idx]->fft.length = length;
  g_graphics->plots[plot_idx]->fft.dx = dx;
  g_graphics->plots[plot_idx]->fft.x0 = x0;
}

char buffer_logger[100];
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *logger(void) {
  sprintf(buffer_logger, "%s", "Some log..");
  return buffer_logger;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void draw_plots_data(void) { g_graphics_ready = true; }

static inline void graphics_plots_init(struct graphics *graphics);

struct graphics *graphics_init(int32_t width, int32_t height, int32_t fps) {
  struct graphics *new_graphics = malloc(sizeof *new_graphics);
  assert(new_graphics);
  new_graphics->width = width;
  new_graphics->height = height;
  new_graphics->width_mid = width / 2;
  new_graphics->height_mid = height / 2;
  new_graphics->fps = fps;
  new_graphics->plots = NULL;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    display_error_sdl("sdl could not init");
    return NULL;
  }

#define WIN_AND_REN 1

#if WIN_AND_REN
  SDL_CreateWindowAndRenderer(new_graphics->width, new_graphics->height, 0,
                              &window, &renderer);
#else
  window = SDL_CreateWindow("Odas", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, new_graphics->width,
                            new_graphics->height, SDL_WINDOW_SHOWN);
#endif

  if (window == NULL) {
    display_error_sdl("window could not be created");
    return NULL;
  }

#if !WIN_AND_REN
  renderer = SDL_CreateRenderer(
      window, -1, 0 /* SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC */);
#endif

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

  new_graphics->service_channel =
      channels_service_init(4, (SDL_Point){.x = 0, .y = 0});
  new_graphics->relay_channel =
      channels_relay_init(2, (SDL_Point){.x = 0, .y = 244 * 2});

  graphics_plots_init(new_graphics);

  return new_graphics;
}

static inline void graphics_plots_init(struct graphics *graphics) {
  graphics->plots = malloc(sizeof(struct plot *) * g_plots_count);

  size_t plot_i = 0;
  for (size_t i = 0; i < graphics->service_channel->count; ++i) {
    graphics->plots[plot_i] =
        ((struct channel_service *)graphics->service_channel->channels[i])
            ->channel->plot0;
    graphics->plots[plot_i + 1] =
        ((struct channel_service *)graphics->service_channel->channels[i])
            ->channel->plot1;
    plot_i += 2;
  }

  for (size_t i = 0; i < graphics->relay_channel->count; ++i) {
    graphics->plots[plot_i] =
        ((struct channel_relay *)graphics->relay_channel->channels[i])
            ->channel->plot0;
    graphics->plots[plot_i + 1] =
        ((struct channel_relay *)graphics->relay_channel->channels[i])
            ->channel->plot1;
    plot_i += 2;
  }
}

void graphics_free(struct graphics *graphics) {
  channels_service_free(graphics->service_channel);
  channels_relay_free(graphics->relay_channel);

  free(graphics->plots);
  free(graphics);
  graphics = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  TTF_Quit();
  SDL_Quit();
}
