#pragma once
#include "relay_channel.h"
#include "service_channel.h"
#include <stdint.h>

struct graphics {
  SDL_Rect position;
  SDL_Point mouse_position;
  int32_t average_width;
  int32_t average_height;
  int32_t fps;

  struct vec_rchannel *vec_relay;
  struct vec_schannel *vec_service;
};

struct graphics *graphics_init(int32_t screen_width, int32_t screen_height,
                               int32_t rendering_fps);
void graphics_free(struct graphics *graphics);

struct plot **graphics_plots_init(struct graphics *graphics);
void graphics_plots_free(struct plot **plots);
size_t index_plot_switch(size_t idx);
