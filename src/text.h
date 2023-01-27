#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <uchar.h>

struct text16 {
  SDL_Rect position;
  SDL_Texture *texture;
  TTF_Font *font;
  SDL_Color color;
  char16_t *content;
};

struct text8 {
  SDL_Rect position;
  SDL_Texture *texture;
  TTF_Font *font;
  SDL_Color color;
  char *content;
};

enum TEXT_FONT_TYPE { TEXT_FONT_REGULAR = 0, TEXT_FONT_BOLD = 1 };
char const *text_get_font_type(enum TEXT_FONT_TYPE type);

struct text16 *text16_init(char const *font_path, int32_t font_size,
                           SDL_Color color, SDL_Point position,
                           char16_t const *content);

void text16_free(struct text16 *text);
void text16_change_content(struct text16 *text, char16_t const *content);
void text16_change_color(struct text16 *text, SDL_Color color);

struct text8 *text8_init(char const *font_path, int32_t font_size,
                         SDL_Color color, SDL_Point position,
                         char const *content);
void text8_free(struct text8 *text);
void text8_change_content(struct text8 *text, char const *content);
void text8_change_color(struct text8 *text, SDL_Color color);
