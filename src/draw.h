#pragma once

void handle_events(void);

#define DRAW_IN_REN(TEXTURE, POS)                                              \
  SDL_RenderCopy(g_renderer, TEXTURE, NULL, POS);
