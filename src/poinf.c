#include "poinf.h"
#include <SDL2/SDL_rect.h>

struct poinf *poinf_init(SDL_Point coord, SDL_Point pos, size_t fft_len) {
  struct poinf *poinf = malloc(sizeof *poinf);
  poinf->coord = coord;
  poinf->pos = pos;

  poinf->fft_data = malloc(sizeof(SDL_FPoint) * fft_len);
  poinf->fft_len = fft_len;

  return poinf;
}

void poinf_free(struct poinf *poinf) {

  free(poinf->fft_data);

  free(poinf);
  poinf = NULL;
}

bool pointf_check_collision(struct poinf *poinf, SDL_Point mouse) {
  for (size_t i = 0; i < poinf->fft_len; ++i) {
    /* if (poinf[i]. == mx && vec_prev[i].y == my) { */
  }

  return false;
}
