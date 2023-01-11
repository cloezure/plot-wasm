#include "text.h"
#include "common_function.h"
#include "global.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

static inline void text_info_update(struct text* text, char const* info) {
  size_t const len = strlen(info) + 1;
  text->info = malloc(sizeof(char) * len);
  strncpy(text->info, info, len);
  text->info[len] = '\0';
}

struct text *text_build(char const *font_path, int32_t font_size,
                           SDL_Color color, SDL_Rect position,
                           char const *info) {
  struct text *new_text = malloc(sizeof *new_text);
  new_text->font = TTF_OpenFont(font_path, font_size);

  if (new_text->font == NULL) {
    display_error_ttf("font not load");
  }

  new_text->color = color;
  SDL_Surface *sur =
      TTF_RenderText_Blended(new_text->font, info, new_text->color);
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

  text_info_update(new_text, info);
  return new_text;
}

void text_free(struct text *text) {
  if (text == NULL) {
    return;
  }

  TTF_CloseFont(text->font);
  text->font = NULL;

  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  free(text->info);
  text->info = NULL;

  free(text);
  text = NULL;
}

void text_change_info(struct text *text, char const *info) {
  if (text == NULL) {
    return;
  }

  SDL_Surface *sur = TTF_RenderText_Solid(text->font, info, text->color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }

  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->texture = SDL_CreateTextureFromSurface(renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  free(text->info);
  text_info_update(text, info);

  SDL_FreeSurface(sur);
}

void text_change_color(struct text *text, SDL_Color color) {
  if (text == NULL) {
    return;
  }

  SDL_Surface *sur = TTF_RenderText_Solid(text->font, text->info, color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }
  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->position.w = sur->w;
  text->position.h = sur->h;

  text->texture = SDL_CreateTextureFromSurface(renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  SDL_FreeSurface(sur);
}
