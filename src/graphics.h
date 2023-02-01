#pragma once
#include "coinf.h"
#include "rchannel.h"
#include "schannel.h"
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
void push_plot_data(int plot_idx, float *data, int length, float dx, float x0);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void off_channel(int channel_idx);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int plot_enter(void);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void off_draw(void);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void change_locale(char const *locale);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int last_press(void);

struct graphics {
  SDL_Rect pos;
  SDL_Point mouse;
  int32_t width_mid;
  int32_t height_mid;
  int32_t fps;
  int last_press;

  struct vec_rchannel *relay;
  struct vec_schannel *service;
};

struct graphics *graphics_init(int32_t width, int32_t height, int32_t fps);
void graphics_free(struct graphics *graphics);

struct plot **graphics_plots_init(struct graphics *graphics);
void graphics_plots_free(struct plot **plots);
