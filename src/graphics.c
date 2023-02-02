#include "graphics.h"
#include "colorscheme.h"
#include "common_functions.h"
#include "global_vars.h"
#include "lang.h"
#include "plot.h"
#include "text.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <string.h>

static inline void index_plots_set(struct plot **plots);

struct plot **graphics_plots_init(struct graphics *graphics) {
  struct plot **plots = malloc(sizeof(struct plot *) * PLOTS_COUNT);

  size_t plot_i = 0;
  for (size_t i = 0; i < graphics->vec_service->count; ++i) {
    plots[plot_i] = graphics->vec_service->channels[i]->plot0;
    plots[plot_i + 1] = graphics->vec_service->channels[i]->plot1;
    plot_i += 2;
  }

  for (size_t i = 0; i < graphics->vec_relay->count; ++i) {
    plots[plot_i] = graphics->vec_relay->channels[i]->plot0;
    plots[plot_i + 1] = graphics->vec_relay->channels[i]->plot1;
    plot_i += 2;
  }

  return plots;
}

void graphics_plots_free(struct plot **plots) { free(plots); }

struct graphics *graphics_init(int32_t width, int32_t height, int32_t fps) {
  struct graphics *new_graphics = malloc(sizeof *new_graphics);
  assert(new_graphics);
  new_graphics->position = (SDL_Rect){.x = 0, .y = 0, .w = width, .h = height};
  new_graphics->average_width = width / 2;
  new_graphics->average_height = height / 2;
  new_graphics->fps = fps;
  new_graphics->mouse_position = (SDL_Point){0, 0};

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    display_error_sdl("sdl could not init");
    return NULL;
  }

  g_window = SDL_CreateWindow("Odas", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, new_graphics->position.w,
                              new_graphics->position.h, SDL_WINDOW_SHOWN);

  if (g_window == NULL) {
    display_error_sdl("window could not be created");
    return NULL;
  }

  g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_PRESENTVSYNC);

  if (g_renderer == NULL) {
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

  new_graphics->vec_service = vec_schannel_init(4, (SDL_Point){.x = 0, .y = 0});
  new_graphics->vec_relay =
      vec_rchannel_init(2, (SDL_Point){.x = 0, .y = 244 * 2});

  struct plot **plots = graphics_plots_init(new_graphics);
  index_plots_set(plots);
  graphics_plots_free(plots);

  g_lang = lang_en();

  return new_graphics;
}

void graphics_free(struct graphics *graphics) {
  assert(graphics);
  vec_schannel_free(graphics->vec_service);
  vec_rchannel_free(graphics->vec_relay);

  free(graphics);

  SDL_DestroyRenderer(g_renderer);
  g_renderer = NULL;

  SDL_DestroyWindow(g_window);
  g_window = NULL;

  lang_free(g_lang);

  TTF_Quit();
  SDL_Quit();
}

static inline void index_plots_set(struct plot **plots) {
  for (size_t i = 1; i <= PLOTS_COUNT; ++i) {
    size_t sw = index_plot_switch(i - 1);
    plots[sw]->index = i;
  }
}

size_t index_plot_switch(size_t idx) {
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
