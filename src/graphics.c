#include "graphics.h"
#include "channel.h"
#include "common_function.h"
#include "global.h"
#include "parse.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps) { g_graphics->fps = fps; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(char const *data) {
  char *decode = base64_decode(data);

  uint32_t spectrums_count = get_uint32(decode, 0);
  uint32_t reports_count = get_uint32(decode, 4);
  int32_t spectrum_config_size = 4 + 4 + (4 * reports_count);
  coordlist coordinate_list = NULL;

  for(ptrdiff_t spec_idx = 0; spec_idx < spectrums_count; ++spec_idx) {
    int32_t offset = 8 + (spec_idx * spectrum_config_size);
    int32_t dx = get_float(decode, offset);
    int32_t x0 = get_float(decode, offset + 4);
    size_t fft_reports_count = 1024;
    float* fft_reports = malloc(sizeof(*fft_reports));


  }

  // for in dots
  // draw plot
  printf("%s\n", decode);
  free(decode);
}

graphics_t *Graphics_init(int32_t width, int32_t height, int32_t fps) {
  graphics_t graphics_init = {.width = width,
                              .height = height,
                              .width_mid = width / 2,
                              .height_mid = height / 2,
                              .fps = fps};
  graphics_t *new_graphics = NULL;
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

#define WIN_AND_REN 0

#if WIN_AND_REN
  SDL_CreateWindowAndRenderer(new_graphics->width, new_graphics->height, 0,
                              &window, &renderer);
#else
  window = SDL_CreateWindow("plot", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, new_graphics->width,
                            new_graphics->height, SDL_WINDOW_SHOWN);
#endif

  if (window == NULL) {
    display_error_sdl("window could not be created");
    return NULL;
  }

#if !WIN_AND_REN
  renderer = SDL_CreateRenderer(
      window, -1, 0 /* SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC */ );
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
  if(!IMG_Init(flags) && flags) {
    display_error_img("Image could not init");
    return NULL;
  }

  new_graphics->service_channel = Channels_init(4, (SDL_Point){.x = 0, .y = 0});
  new_graphics->relay_channel = Channels_init(2, (SDL_Point){.x = 0, .y = 244*2});

  return new_graphics;
}

void Graphics_free(graphics_t *graphics) {
  Channels_free(graphics->service_channel);
  Channels_free(graphics->relay_channel);
  free(graphics);
  graphics = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  TTF_Quit();
  SDL_Quit();
}
