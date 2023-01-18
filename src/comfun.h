#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

static inline void display_error_sdl(char const *info) {
  printf("%s! SDL_Error: %s\n", info, SDL_GetError());
}

static inline void display_error_ttf(char const *info) {
  printf("%s! TTF_Error: %s\n", info, TTF_GetError());
}

static inline void display_error_img(char const *info) {
  printf("%s! IMG_Error: %s\n", info, IMG_GetError());
}

#define REPEAT(count)                                                          \
  for (size_t hidden_idx = 0; hidden_idx < count; ++hidden_idx)

#define LOOP for (;;)
#define UNUSED (void)

static inline bool is_even(int32_t number) { return number % 2 == 0; }

static inline bool is_odd(int32_t number) { return number % 2 != 0; }

static inline bool check_zero_array(float *data, size_t length) {
  REPEAT(length) {
    if (*data != 0.0f) {
      return false;
    }
    ++data;
  }
  return true;
}
