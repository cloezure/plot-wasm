#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uchar.h>
#include <wchar.h>

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

static inline bool check_zero_array(SDL_FPoint *data, size_t length) {
  REPEAT(length) {
    if ((*data).y != 0.0f) {
      return false;
    }
    ++data;
  }
  return true;
}

static inline int strlen16(char16_t const *str) {
  if (!str)
    return -1;

  char16_t const *in_str = str;
  for (; *in_str; ++in_str)
    ;

  return in_str - str;
}

static inline char16_t *strcpy16(char16_t *dest, char16_t const *src) {
  char16_t *tmp = dest;
  while ((*tmp++ = *src++) != 0)
    ;
  return dest;
}

static inline int sprintf16_(char16_t *dest, size_t len, char16_t const *src,
                             ...) {
  // to lazy...
  strcpy16(dest, u"0");
  return 0;
}

/* printf("ch = %zu wch = %zu\n", sizeof(char16_t), sizeof(wchar_t)); \ */
/* static_assert(sizeof(char16_t) != sizeof(wchar_t), "wchar_t != char16_t\n");
 * \ */
#define sprintf16(DEST, LEN, SRC, ...)                                         \
  if (sizeof(char16_t) != sizeof(wchar_t)) {                                   \
    sprintf16_(DEST, LEN, SRC, __VA_ARGS__);                                   \
  } else {                                                                     \
    swprintf((wchar_t *)DEST, LEN, (wchar_t const *)SRC, __VA_ARGS__);         \
  }
