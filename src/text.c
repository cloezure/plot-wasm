#include "text.h"
#include "common_function.h"
#include "global.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

char const *font_type(enum FONT_TYPE type) {
  switch (type) {
  case FONT_R:
#ifdef __EMSCRIPTEN__
    return "res/Gilroy-Regular.ttf";
#else
    return "/home/nim/dev/c/plot-wasm/res/Gilroy-Regular.ttf";
#endif
  case FONT_B:
#ifdef __EMSCRIPTEN__
    return "res/Gilroy-Bold.ttf";
#else
    return "/home/nim/dev/c/plot-wasm/res/Gilroy-Bold.ttf";
#endif
  }
}

Text *Text_init(char const *font_path, int32_t font_size, SDL_Color color,
                SDL_Rect position, char const *text) {
  Text *new_text = malloc(sizeof(*new_text));
  new_text->font = TTF_OpenFont(font_path, font_size);

  if (new_text->font == NULL) {
    display_error_ttf("font not load");
  }

  new_text->color = color;
  SDL_Surface *sur =
      TTF_RenderText_Blended(new_text->font, text, new_text->color);
  if (sur == NULL) {
    display_error_sdl("surface could not be created");
  }

  new_text->position.x = position.x;
  new_text->position.y = position.y;
  new_text->position.w = sur->w;
  new_text->position.h = sur->h;

  new_text->texture = SDL_CreateTextureFromSurface(renderer, sur);
  if (new_text->texture == NULL) {
    display_error_sdl("texture could not be create by surface");
  }

  SDL_FreeSurface(sur);

  return new_text;
}

void Text_free(Text *text) {
  if (text == NULL) {
    return;
  }

  if (text->font != NULL) {
    TTF_CloseFont(text->font);
    text->font = NULL;
  }

  if (text->texture != NULL) {
    SDL_DestroyTexture(text->texture);
    text->texture = NULL;
  }

  free(text);
  text = NULL;
}

void Text_set_text(Text *text, char const *info) {
  if (text->font == NULL || text == NULL) {
    return;
  }

  SDL_Surface *sur = TTF_RenderText_Solid(text->font, info, text->color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }

  if (text->texture == NULL) {
    SDL_FreeSurface(sur);
    return;
  }

  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->texture = SDL_CreateTextureFromSurface(renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  SDL_FreeSurface(sur);
}
