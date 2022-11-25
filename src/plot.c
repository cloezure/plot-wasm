#include <SDL2/SDL_render.h>
#include <assert.h>
#include <stdlib.h>

#include "coordsinfo.h"
#include "plot.h"
#include "globalplotvar.h"

SDL_Point mouse_pos = {.x = 0, .y = 0};
uint32_t ticks_for_next_down = 0;

static inline void load_coords_info(void) {
    SDL_Color color = {0xFF, 0xFF, 0xFF, 255};
    SDL_Rect position = {.x = WIN_WIDTH - 100, .y = 20, .w = 50, .h = 20};
    g_coords_info = CoordsInfo_init("res/LiberationSerif-Regular.ttf", 20, color, position);
}

static inline void unload_coords_info(void) {
    CoordsInfo_free(g_coords_info);
}

static void math_coords_to_screen(double x, double y, double scale, int cx, int cy, int*sx, int*sy) {
    assert(sx != NULL);
    assert(sy != NULL);

    *sx = cx + x * scale;
    *sy = cy - y * scale;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_spec(float* specs, int32_t spec_count, int32_t reports_count, size_t spec_config_size) {

}

static inline void draw_background(void) {
    SDL_SetRenderDrawColor(g_ren, 0x20, 0x20, 0x20, 0xFF);
    SDL_RenderClear(g_ren);
}

static inline void draw_coords_lines(void) {
    SDL_SetRenderDrawColor(g_ren, 0xFF, 0x00, 0x00, 0xFF);

    /* x */
    SDL_RenderDrawLine(g_ren, 0, WIN_MID_HEIGHT, WIN_WIDTH, WIN_MID_HEIGHT);

    /* y */
    SDL_RenderDrawLine(g_ren, WIN_MID_WIDTH, 0, WIN_MID_WIDTH, WIN_HEIGHT);
}

static inline void draw_coords_info(void) {
    CoordsInfo_update_coords(g_coords_info, mouse_pos.x, mouse_pos.y);
    SDL_RenderCopy(g_ren, g_coords_info->texture, NULL, &g_coords_info->position);
}

static inline void draw(void) {
    draw_background();
    draw_coords_lines();
    draw_coords_info();

    SDL_RenderPresent(g_ren);
}


void init_sdl_environment(void) {

    // Video Engine
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DISPLAY_ERROR_SDL("SDL could not init!");
        exit(EXIT_FAILURE);
    }


#define REN_AND_WIN 1

#if REN_AND_WIN
    SDL_CreateWindowAndRenderer(WIN_WIDTH,
                                WIN_HEIGHT,
                                0,
                                &g_win,
                                &g_ren);
#else
    // Window
    g_win = SDL_CreateWindow("Plot",
                           SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED,
                           WIN_WIDTH,
                           WIN_HEIGHT,
                           SDL_WINDOW_SHOWN);
#endif

    if (g_win == NULL) {
        DISPLAY_ERROR_SDL("Window could not be created!");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

#if !REN_AND_WIN

    // Graphics Renderer
    g_ren = SDL_CreateRenderer(g_win,
                             -1,
                             SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
#endif

    if (g_ren == NULL) {
        DISPLAY_ERROR_SDL("Renderer could not be created!");
        exit(EXIT_FAILURE);
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("%s\n",
               "Warning: Linear texture filtering not enabled!");
    }

    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    load_coords_info();
    SDL_SetRenderDrawColor(g_ren, 0xFF, 0xFF, 0xFF, 0xFF);
}

void clear_sdl_environment(void) {

    SDL_DestroyRenderer(g_ren);
    SDL_DestroyWindow(g_win);
    unload_coords_info();
    g_win = NULL;
    g_ren = NULL;

    TTF_Quit();
    SDL_Quit();
}

void handle_events(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT: {
            clear_sdl_environment();
            exit(EXIT_SUCCESS);
            break;
        }
        case SDL_MOUSEMOTION: {
            mouse_pos.x = event.motion.x;
            mouse_pos.y = event.motion.y;
            break;
        }
    }

    draw();
}
