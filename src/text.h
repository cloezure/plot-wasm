#pragma once

#include <SDL2/SDL_ttf.h>
#include <stdint.h>

typedef struct Text {
  TTF_Font *font;
  SDL_Texture *texture;
  SDL_Color color;
  SDL_Rect position;
} Text;

enum FONT_TYPE { FONT_R, FONT_B };

char const *font_type(enum FONT_TYPE type);

Text *Text_init(char const *font_path, int32_t font_size, SDL_Color color,
                SDL_Rect position, char const *text);
void Text_free(Text *text);

void Text_set_text(Text *text, char const *info);
