#include "graphics.h"
#include "channel.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"
#include "text.h"
#include "parse.h"

#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps) { graphics->fps = fps; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(char const *data) {
  char *decode = base64_decode(data);

  uint32_t spectrums_count = get_uint32(decode, 0);
  uint32_t reports_count = get_uint32(decode, 4);
  int32_t spectrum_config_size = 4 + 4 + (4 * reports_count);

  /* for(ptrdiff_t spec_idx = 0; spec_idx < spectrums_count; ++spec_idx) { */
  /*   int32_t offset = 8 + (spec_idx * spectrum_config_size); */
  /*   int32_t dx = get_float(decode, offset); */
  /*   int32_t x0 = get_float(decode, offset + 4); */
  /*   size_t fft_reports_count =  */
  /*   float* fft_reports = malloc(sizeof(*fft_reports)); */

  /* } */

  // for in dots
  // draw plot
  printf("%s\n", decode);
  free(decode);
}

static inline void draw_background(SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderClear(renderer);
}


static inline void draw_fps(void) {
  SDL_Rect pos = {.y = 10};
  char buff[100];
  sprintf(buff, "%d", graphics->fps);

  text_t * fps = Text_init(font_type(FONT_B), 60, COLOR_GREEN, pos, buff);
  fps->position.x = graphics->width - fps->position.w - 10;

  SDL_RenderCopy(renderer, fps->texture, NULL, &fps->position);
  Text_free(fps);
}

static inline void draw_channels(void) {
  SDL_RenderCopy(renderer, graphics->channel->plot0->background, NULL,
                 &graphics->channel->plot0->position);
  SDL_RenderCopy(renderer, graphics->channel->plot1->background, NULL,
                 &graphics->channel->plot1->position);
  SDL_RenderCopy(renderer, graphics->channel->channel_number->texture, NULL,
                 &graphics->channel->channel_number->position);
  SDL_RenderCopy(renderer, graphics->channel->plot0_name->texture, NULL,
                 &graphics->channel->plot0_name->position);
  SDL_RenderCopy(renderer, graphics->channel->plot1_name->texture, NULL,
                 &graphics->channel->plot1_name->position);
}

static inline void draw(void) {
  draw_background(COLOR_BACKGROUND);
  draw_channels();
  draw_fps();
  SDL_RenderPresent(renderer);
}

void handle_events(void) {

  const int32_t frame_delay = 1000 / graphics->fps;
  uint32_t frame_start;
  int32_t frame_time;

  frame_start = SDL_GetTicks();

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

  frame_time = SDL_GetTicks() - frame_start;

  if (frame_delay > frame_time) {
    SDL_Delay(frame_delay - frame_time);
  }

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
    display_error_ttf("ttf could not init");
    return NULL;
  }

  SDL_Point channel_pos = { .x = 0, .y = 0};
  new_graphics->channel = Channel_init(channel_pos, "1",
                                       "Tx", "Rx");

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
