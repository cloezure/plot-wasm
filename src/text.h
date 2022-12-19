#pragma once

#include <SDL2/SDL_ttf.h>
#include <stdint.h>

typedef struct Text {
  TTF_Font *font;
  SDL_Texture *texture;
  SDL_Color color;
  SDL_Rect position;
} text_t;

text_t *Text_init(char const *font_path, int32_t font_size, SDL_Color color,
                  SDL_Rect position, char const *text);
void Text_free(text_t *text);

void Text_set_text(text_t *text, char const *info);
