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
void push_data(float *fft, int size, int plot_idx, float dx, float x0);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char *logger(void);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void draw_plots_data(void);

typedef struct Graphics {
  int32_t const width;
  int32_t const height;
  int32_t const width_mid;
  int32_t const height_mid;
  int32_t fps;
  Plot **plots;

  Channels *service_channel;
  Channels *relay_channel;
} Graphics;

Graphics *Graphics_init(int32_t width, int32_t height, int32_t fps);
void Graphics_free(Graphics *graphics);
