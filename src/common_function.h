#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

static inline void display_error_sdl(char const* info) {
  printf("%s! SDL_Error: %s\n", info, SDL_GetError());
}

static inline void display_error_ttf(char const* info) {
  printf("%s! TTF_Error: %s\n", info, TTF_GetError());
}

static inline void display_error_img(char const* info) {
  printf("%s! IMG_Error: %s\n", info, IMG_GetError());
}

#define __REPEAT__(COUNT)                       \
  for(ptrdiff_t i = 0; i < COUNT; ++i)

static inline bool iseven(int32_t number) {
  return number % 2 == 0;
}

static inline bool isodd(int32_t number) {
  return number % 2 != 0;
}
