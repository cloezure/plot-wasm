#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

struct text {
  SDL_Rect position;
  SDL_Texture *texture;
  TTF_Font *font;
  char *content;
  SDL_Color color;
};

enum TEXT_FONT_TYPE { TEXT_FONT_REGULAR = 0, TEXT_FONT_BOLD = 1 };
char const *text_get_font_type(enum TEXT_FONT_TYPE type);

struct text *text_cons(char const *font_path, int32_t font_size,
                       SDL_Color color, SDL_Point position,
                       char const *content);

void text_free(struct text *text);
void text_change_content(struct text *text, char const *content);
void text_change_color(struct text *text, SDL_Color color);
