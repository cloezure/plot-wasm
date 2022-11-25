#include <SDL2/SDL.h>
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "globalplotvar.h"
#include "plot.h"

int32_t main(void) {
    init_sdl_environment();
#ifdef __EMSCRIPTEN__
    const int32_t simulate_infinite_loop = 1;
    const int32_t fps = -1;

    /* emscripten_set_some_callback("canvas0", NULL, EM_FALSE, NULL); */
    emscripten_set_main_loop(handle_events, fps, simulate_infinite_loop);
#else
    while(1) {
        handle_events();
    }
#endif
    clear_sdl_environment();
}
