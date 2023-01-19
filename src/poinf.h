#pragma once

#include <SDL2/SDL_rect.h>
#include <stdbool.h>

struct poinf {
  SDL_Point coord;
  SDL_Point pos;
  SDL_FPoint *fft_data;
  size_t fft_len;
};

struct poinf *poinf_init(SDL_Point coord, SDL_Point pos, size_t fft_len);
void poinf_free(struct poinf *poinf);

bool pointf_check_collision(struct poinf *poinf, SDL_Point mouse);
