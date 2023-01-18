#pragma once
#include "channel.h"
#include "coord_info.h"
#include "vec_channel.h"
#include <SDL2/SDL.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void trans_plot_data(int plot_idx, float *data, int length, float dx, float x0);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *logger(void);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void off_channel(int channel_idx);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void window_size(int w, int h);

struct graphics {
  SDL_Rect pos;
  int32_t width_mid;
  int32_t height_mid;
  int32_t fps;
  /* struct vec_coord_info *vec_coord_info; */

  struct vec_channel *relay;
  struct vec_channel *service;
};

struct graphics *graphics_cons(int32_t width, int32_t height, int32_t fps);
void graphics_free(struct graphics *graphics);

struct plot **graphics_plots_cons(struct graphics *graphics);
void graphics_plots_free(struct plot **plots);
