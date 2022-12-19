#include <assert.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "global.h"
#include "graphics.h"

int32_t main(void) {
  int32_t const w = 800;
  int32_t const h = 800;
  int32_t const fps_draw = 60;

  graphics = Graphics_init(w, h, fps_draw);
  if (graphics == NULL) {
    return EXIT_FAILURE;
  }

#ifdef __EMSCRIPTEN__
  int32_t const simulate_infinite_loop = 1;
  int32_t const fps = -1;
  /* emscripten_set_some_callback("canvas0", NULL, EM_FALSE, NULL); */
  emscripten_set_main_loop(handle_events, fps, simulate_infinite_loop);
#else
  while (1) {
    handle_events();
  }
#endif
  Graphics_free(graphics);
}
