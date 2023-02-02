#include "js_api.h"
#include "global_vars.h"

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void off_channel(int channel_idx) {
  if (channel_idx < 0 || channel_idx >= CHANNELS_COUNT)
    return;

  int const serv_count = g_graphics->vec_service->count - 1;
  if (channel_idx <= serv_count) {
    off_schannel(g_graphics->vec_service, channel_idx);
  } else {
    off_rchannel(g_graphics->vec_relay, channel_idx - 4);
  }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void on_channel(int channel_idx) {
  if (channel_idx < 0 || channel_idx >= CHANNELS_COUNT)
    return;

  int const serv_count = g_graphics->vec_service->count - 1;
  if (channel_idx <= serv_count) {
    on_schannel(g_graphics->vec_service, channel_idx);
  } else {
    on_rchannel(g_graphics->vec_relay, channel_idx - 4);
  }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int plot_enter(void) { return 0; }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void off_draw(void) {
  for (size_t i = 0; i < CHANNELS_COUNT; ++i) {
    off_channel(i);
  }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void on_draw(void) {
  for (size_t i = 0; i < CHANNELS_COUNT; ++i) {
    on_channel(i);
  }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void change_locale(char const *local) { change_lang(&g_lang, local); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int fps) {
  if (fps <= 0)
    return;

  g_graphics->fps = fps;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int last_press(void) {
  int v = g_last_press;
  g_last_press = 0;
  return v;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_plot_data(int plot_idx, float *data, int length, float dx, float x0) {
  if (plot_idx < 0 || plot_idx > PLOTS_COUNT)
    return;

  plot_idx = index_plot_switch(plot_idx);

  struct plot **plots = graphics_plots_init(g_graphics);
  plot_fft_update(plots[plot_idx], data, length, dx / (100000000 - 25000000),
                  x0 / 10000);
  graphics_plots_free(plots);
}
