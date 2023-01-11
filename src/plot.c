#include "plot.h"
#include "common_function.h"
#include "global.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>

struct plot *plot_build(SDL_Point position) {

#if BMP_BACKGROUND_FOR_PLOT
  SDL_Surface *sur = SDL_LoadBMP("res/plot_back.bmp");
  if (sur == NULL) {
    display_error_sdl("Can't load plot_back.bmp");
    return NULL;
  }

  uint32_t color_key = SDL_MapRGB(sur->format, 0xFF, 0xFF, 0xFF);
  SDL_SetColorKey(sur, SDL_TRUE, color_key);

#else
  SDL_Surface *sur = IMG_Load(PLOT_BACKGROUND_PNG);
  if (sur == NULL) {
    display_error_img("Can't load plot_back.png");
    return NULL;
  }
#endif

  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
  if (tex == NULL) {
    display_error_sdl("Can't create texture from surface plot_back");
    SDL_FreeSurface(sur);
    return NULL;
  }

  struct plot *new_plot = malloc(sizeof *new_plot);
  new_plot->position.x = position.x;
  new_plot->position.y = position.y;
  new_plot->position.w = sur->w;
  new_plot->position.h = sur->h;
  new_plot->background = tex;
  new_plot->fft.dx = 0.0f;
  new_plot->fft.x0 = 0.0f;
  new_plot->fft.length = 0;
  new_plot->fft.data = malloc(sizeof(float) * 1);

  SDL_FreeSurface(sur);

  return new_plot;
}

void plot_free(struct plot *plot) {
  if (plot == NULL) {
    printf("plot == NULL");
    return;
  }

  SDL_DestroyTexture(plot->background);
  plot->background = NULL;

  free(plot->fft.data);

  free(plot);
  plot = NULL;
}
