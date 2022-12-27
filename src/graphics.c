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
void set_fps(int fps) { g_graphics->fps = fps; printf("FPS be set %d", fps);}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(char const *data) {
  if(g_graphics->data_graphics != NULL) {
    free(g_graphics->data_graphics);
    g_graphics->data_graphics = NULL;
  }

  size_t const len = strlen(data);
  g_graphics->data_graphics = malloc(sizeof(char) * len);
  strncpy(g_graphics->data_graphics, data, len);
  flag_draw = true;
}

graphics_t *Graphics_init(int32_t width, int32_t height, int32_t fps) {
  graphics_t graphics_init = {.width = width,
                              .height = height,
                              .width_mid = width / 2,
                              .height_mid = height / 2,
                              .fps = fps,
                              .data_graphics = NULL};

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

  new_graphics->service_channel = Channels_service_init(4, (SDL_Point){.x = 0, .y = 0});
  new_graphics->relay_channel = Channels_relay_init(2, (SDL_Point){.x = 0, .y = 244*2});

  return new_graphics;
}

void Graphics_free(graphics_t *graphics) {
  Channels_service_free(graphics->service_channel);
  Channels_relay_free(graphics->relay_channel);
  free(graphics);
  graphics = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  TTF_Quit();
  SDL_Quit();
}
