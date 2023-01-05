#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <assert.h>

#include "common_function.h"
#include "draw.h"
#include "global.h"
#include "graphics.h"

int main(void) {
  int32_t const w = 1308;
  int32_t const h = 734;
  int32_t const fps_draw = 60;

  g_graphics = graphics_crealloc(w, h, fps_draw);
  assert(g_graphics);

#ifdef __EMSCRIPTEN__
  int32_t const simulate_infinite_loop = 1;
  int32_t const fps = -1;
  /* emscripten_set_some_callback("canvas0", NULL, EM_FALSE, NULL); */
  emscripten_set_main_loop(handle_events, fps, simulate_infinite_loop);
#else
  LOOP { handle_events(); }
#endif
  /* Graphics_free(graphics); */
}
