#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <assert.h>
#include <stdlib.h>

#include "common_functions.h"
#include "draw.h"
#include "global_vars.h"
#include "graphics.h"
#include "locale.h"

#define VERSION 1.8b
#define AUTHORS EZQB

int main(void) {
  setlocale(LC_ALL, "Russian");
  int32_t const screen_width = 1308;
  int32_t const screen_height = 734;
  int32_t const rendering_fps = 60;

  g_graphics = graphics_init(screen_width, screen_height, rendering_fps);
  assert(g_graphics);

#ifdef __EMSCRIPTEN__
  int32_t const simulate_infinite_loop = 1;
  int32_t const fps = -1;
  emscripten_set_main_loop(handle_events, fps, simulate_infinite_loop);
#else
  LOOP { handle_events(); }
#endif
  graphics_free(g_graphics);
  return EXIT_SUCCESS;
}
