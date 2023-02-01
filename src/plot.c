#include "plot.h"
#include "chart_points.h"
#include "colorscheme.h"
#include "comfun.h"
#include "global.h"
#include "text.h"
#include <SDL2/SDL_rect.h>
#include <assert.h>

char const plot_background_png[] = "res/plot_back.png";

struct plot *plot_init(SDL_Point position, char16_t const *name) {

  SDL_Surface *sur = IMG_Load(plot_background_png);
  if (sur == NULL) {
    display_error_img("Can't load plot_back.png");
    return NULL;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, sur);
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
  new_plot->fft.data = malloc(sizeof(SDL_FPoint));
  new_plot->index = 0;

  int32_t const plot_size_name = 20;
  SDL_Point pos_name = {.x = position.x + 9, .y = position.y - 11};
  new_plot->name = text16_init(text_get_font_type(TEXT_FONT_BOLD),
                               plot_size_name, COLOR_PLOT_NAME, pos_name, name);

  size_t hlpoints = 7;
  int32_t hj = -30;
  int32_t *hpoints = malloc(sizeof *hpoints * hlpoints);
  for (size_t i = 0; i < hlpoints; ++i) {
    hpoints[i] = hj;
    hj += 10;
  }
  SDL_Point hcharts_pos = {.x = position.x, .y = position.y + sur->h + 5};
  new_plot->hcharts = chart_points_init(u"MHz", hpoints, hlpoints, hcharts_pos,
                                        CHARTS_MODE_H, 30);

  size_t vlpoints = 6;
  int32_t vj = 0;
  int32_t *vpoints = malloc(sizeof *vpoints * vlpoints);
  for (size_t i = 0; i < vlpoints; ++i) {
    vpoints[i] = vj;
    vj -= 20;
  }
  SDL_Point vcharts_pos = {.x = position.x - 34, .y = position.y};
  new_plot->vcharts = chart_points_init(u"dB", vpoints, vlpoints, vcharts_pos,
                                        CHARTS_MODE_V, 28);

  SDL_FreeSurface(sur);
  free(hpoints);
  free(vpoints);

  return new_plot;
}

void plot_free(struct plot *plot) {
  assert(plot);

  SDL_DestroyTexture(plot->background);
  plot->background = NULL;

  free(plot->fft.data);
  text16_free(plot->name);
  chart_points_free(plot->hcharts);
  chart_points_free(plot->vcharts);

  free(plot);
}

void plot_fft_update(struct plot *plot, float *data, int length, float dx,
                     float x0) {
  free(plot->fft.data);
  plot->fft.data = malloc(sizeof(SDL_FPoint) * length);

  for (int i = 0; i < length; ++i) {
    plot->fft.data[length - 1 - i].y = data[i];
  }

  plot->fft.length = length;
  plot->fft.dx = dx;
  plot->fft.x0 = x0;
}
