#pragma once
#include "channel.h"
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
void push_data(int plot_idx, float *data, int length, float dx, float x0);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *logger(void);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void draw_plots_data(void);

struct graphics {
  int32_t width;
  int32_t height;
  int32_t width_mid;
  int32_t height_mid;
  int32_t fps;
  struct plot **plots;

  struct channels *service_channel;
  struct channels *relay_channel;
};

struct graphics *graphics_crealloc(int32_t width, int32_t height, int32_t fps);
void graphics_free(struct graphics *graphics);
