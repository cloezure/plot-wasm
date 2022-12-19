#include "graphics.h"
#include "channel.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"

#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int32_t fps) { graphics->fps = fps; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(char const *data) {
  // decode base64
  // for in dots
  // draw plot
}

static inline void draw_background(SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer);
}

static inline void draw_channels(void) {
  SDL_RenderCopy(renderer, graphics->channel->channel_number->texture, NULL,
                 &graphics->channel->channel_number->position);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_channels();
  SDL_RenderPresent(renderer);
}

void handle_events(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT: {
    Graphics_free(graphics);
    exit(EXIT_SUCCESS);
    break;
  }
  }

  draw();
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
      window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
#endif

  if (renderer == NULL) {
    display_error_sdl("renderer could not be created");
    return NULL;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    puts("Warning: linear texture filtering not enabled!");
  }

  if (TTF_Init() < 0) {
    display_error_ttf("ttf could not init");
    return NULL;
  }

  new_graphics->channel =
      Channel_init((SDL_Rect){.h = 100, .w = 100, .x = 10, .y = 10}, "1",
                   "Serv Tx0", "Serv Tx1");

  return new_graphics;
}

void Graphics_free(graphics_t *graphics) {
  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  // free channels
  Channel_free(graphics->channel);

  TTF_Quit();
  SDL_Quit();
}
