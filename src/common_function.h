#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static inline void display_error_sdl(char const* info) {
  printf("%s! SDL_Error: %s\n", info, SDL_GetError());
}

static inline void display_error_ttf(char const* info) {
  printf("%s! TTF_Error: %s\n", info, TTF_GetError());
}

#define __REPEAT__(COUNT)                       \
  for(ptrdiff_t i = 0; i < COUNT; ++i)
