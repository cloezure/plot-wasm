#pragma once

void handle_events(void);

#define _draw_in_ren_(TEXTURE, POS)                                            \
  SDL_RenderCopy(renderer, TEXTURE, NULL, POS);\
