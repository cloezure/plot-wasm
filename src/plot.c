#include <SDL2/SDL_render.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "coordsinfo.h"
#include "plot.h"
#include "globalplotvar.h"

SDL_Point mouse_pos = {.x = 0, .y = 0};

bool Plot_init(void) {
    // Video Engine
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        DISPLAY_ERROR_SDL("SDL could not init!");
        return false;
    }

    SDL_CreateWindowAndRenderer(WIN_WIDTH,
                                WIN_HEIGHT,
                                0,
                                &g_win,
                                &g_ren);

    if (g_win == NULL) {
        DISPLAY_ERROR_SDL("Window could not be created!");
        SDL_Quit();
        return false;
    }

    if (g_ren == NULL) {
        DISPLAY_ERROR_SDL("Renderer could not be created!");
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        printf("%s\n",
               "Warning: Linear texture filtering not enabled!");
    }

    if(TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    SDL_SetRenderDrawColor(g_ren, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}

bool Plot_load(void) {
    /* load coords info */
    SDL_Color color = {0xFF, 0xFF, 0xFF, 255};
    SDL_Rect position = {.x = WIN_WIDTH - 100, .y = 20, .w = 50, .h = 20};
    g_coords_info = CoordsInfo_init("res/Gilroy-Regular.ttf", 20, color, position);

    return true;
}

void Plot_quit(void) {
    SDL_DestroyRenderer(g_ren);
    g_ren = NULL;
    SDL_DestroyWindow(g_win);
    g_win = NULL;

    CoordsInfo_free(g_coords_info);
    g_coords_info = NULL;

    TTF_Quit();
    SDL_Quit();
}

static uint32_t get_uint32(char const* data, size_t offset) {
    uint32_t res = 0;
    memcpy(&res, data + offset, sizeof(res));
    return res;
}

static float get_float(char const* data, size_t offset) {
    float res = 0.0f;
    memcpy(&res, data + offset, sizeof(res));
    return res;
}

static char* slice(char const* str, char * res, size_t start, size_t end) {
    return strncpy(res, str + start, end - start);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_fps(int32_t fps) {
    FPS_PLOT = fps;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void push_data(char *data) {
    // parse base64
    // for on string to push in traces
}

/* void push_spec(char const* view) { */
/*     uint32_t const spectrums_count = get_uint32(view, 0); */
/*     uint32_t const reports_count = get_uint32(view, 4); */

/*     /\* {x0, dx, spectrum} *\/ */
/*     size_t const spectrum_config_size = 4 + 4 + (4 * reports_count); */

/*     for(size_t spec_idx = 0; spec_idx < spectrums_count; ++spec_idx) { */
/*         size_t const offset = 8 + (spec_idx * spectrum_config_size); */
/*         char buffer[100] = {0}; */
/*         spec_t * spec = Spectrum_init(strlen(slice(view, buffer, offset + 8, offset + spectrum_config_size))); */

/*     } */
/* } */

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

/* static inline void draw_plot() { */
/*     assert(g_trace != NULL); */
/*     size_t report_idx = 0; */
/*     SDL_SetRenderDrawColor(g_ren, 0x00, 0x00, 0xFF, 0xFF); */
/*     for(float i = g_trace->x0; i < g_trace->fft_reports_size; i += g_trace->dx) { */
/*         SDL_RenderDrawLineF(g_ren, */
/*                             i, */
/*                             g_trace->fft_reports[report_idx] + 100, */
/*                             i + g_trace->dx, */
/*                             g_trace->fft_reports[report_idx] + 100); */
/*     } */
/* } */


static inline void draw(void) {
    draw_background();
    draw_coords_lines();
    draw_coords_info();

    /* if(g_draw_plot) { */
    /*     draw_plot(); */
    /* } */

    SDL_RenderPresent(g_ren);
}

void handle_events(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT: {
            return;
        }
        case SDL_MOUSEMOTION: {
            mouse_pos.x = event.motion.x - WIN_MID_WIDTH;
            mouse_pos.y = event.motion.y - WIN_MID_HEIGHT;
            break;
        }
    }

    draw();
}
