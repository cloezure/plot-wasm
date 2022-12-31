#include "graphics.h"
#include "channel.h"
#include "common_function.h"
#include "global.h"
#include "parse.h"
#include "plot.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps) { g_graphics->fps = fps; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(float *fft, int size, int plot_idx, float dx, float x0) {
  g_graphics->plots[plot_idx]->fft = fft;
  g_graphics->plots[plot_idx]->fft_len = size;
  g_graphics->plots[plot_idx]->dx = dx;
  g_graphics->plots[plot_idx]->x0 = x0;
}

char buffer_logger[100];
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *logger(void) {
  double sum = 0;
  for (size_t i = 0; i < g_graphics->plots[1]->fft_len; ++i) {
    sum += g_graphics->plots[1]->fft[i];
  }
  sprintf(buffer_logger, "%lf", sum);
  return buffer_logger;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void draw_plots_data(void) { g_graphics_ready = true; }

static inline void Graphics_plots_init(Graphics *graphics);

Graphics *Graphics_init(int32_t width, int32_t height, int32_t fps) {
  Graphics graphics_init = {
      .width = width,
      .height = height,
      .width_mid = width / 2,
      .height_mid = height / 2,
      .fps = fps,
      .plots = NULL,
  };

  Graphics *new_graphics = NULL;
  new_graphics = malloc(sizeof(*new_graphics));

  if (new_graphics == NULL) {
    puts("malloc could not allocate memory for graphics");
    return NULL;
  }
  memcpy(new_graphics, &graphics_init, sizeof(*new_graphics));

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
      Channels_service_init(4, (SDL_Point){.x = 0, .y = 0});
  new_graphics->relay_channel =
      Channels_relay_init(2, (SDL_Point){.x = 0, .y = 244 * 2});

  Graphics_plots_init(new_graphics);

  return new_graphics;
}

static inline void Graphics_plots_init(Graphics *graphics) {
  graphics->plots = malloc(sizeof(Plot *) * g_plots_count);

  size_t plot_i = 0;
  for (size_t i = 0; i < graphics->service_channel->count; ++i) {
    graphics->plots[plot_i] =
        ((Channel_service *)graphics->service_channel->channels[i])
            ->channel->plot0;
    graphics->plots[plot_i + 1] =
        ((Channel_service *)graphics->service_channel->channels[i])
            ->channel->plot1;
    plot_i += 2;
  }

  for (size_t i = 0; i < graphics->relay_channel->count; ++i) {
    graphics->plots[plot_i] =
        ((Channel_relay *)graphics->relay_channel->channels[i])->channel->plot0;
    graphics->plots[plot_i + 1] =
        ((Channel_relay *)graphics->relay_channel->channels[i])->channel->plot1;
    plot_i += 2;
  }
}

void Graphics_free(Graphics *graphics) {
  Channels_service_free(graphics->service_channel);
  Channels_relay_free(graphics->relay_channel);

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
