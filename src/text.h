#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#define TEXT_FONT_REGULAR "res/Gilroy-Regular.ttf"
#define TEXT_FONT_BOLD "res/Gilroy-Bold.ttf"

struct text {
  TTF_Font *font;
  SDL_Texture *texture;
  char* info;
  SDL_Color color;
  SDL_Rect position;
};

struct text *text_build(char const *font_path, int32_t font_size,
                           SDL_Color color, SDL_Rect position,
                           char const *info);
void text_free(struct text *text);

void text_change_info(struct text *text, char const *info);
void text_change_color(struct text* text, SDL_Color color);
