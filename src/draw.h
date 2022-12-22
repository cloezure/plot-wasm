#pragma once

void handle_events(void);

#define DRAW_IN_REN(TEXTURE, POS)               \
    SDL_RenderCopy(renderer,                    \
                   TEXTURE,                     \
                   NULL,                        \
                   POS);                        \
