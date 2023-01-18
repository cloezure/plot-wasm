#include "text.h"
#include "common_function.h"
#include "global.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

static inline void text_content_update(struct text *text, char const *content);

char const *text_get_font_type(enum TEXT_FONT_TYPE type) {
  switch (type) {
  case TEXT_FONT_REGULAR:
    return "res/Gilroy-Regular.ttf";
  case TEXT_FONT_BOLD:
    return "res/Gilroy-Bold.ttf";
  default:
    return "res/Gilroy-Regular.ttf";
  }
}

struct text *text_init(char const *font_path, int32_t font_size,
                       SDL_Color color, SDL_Point position,
                       char const *content) {
  struct text *new_text = malloc(sizeof *new_text);
  new_text->font = TTF_OpenFont(font_path, font_size);

  if (new_text->font == NULL) {
    display_error_ttf("font not load");
  }

  new_text->color = color;
  SDL_Surface *sur =
      TTF_RenderText_Blended(new_text->font, content, new_text->color);
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

  text_content_update(new_text, content);
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

  free(text->content);
  text->content = NULL;

  free(text);
  text = NULL;
}

void text_change_content(struct text *text, char const *content) {
  if (text == NULL) {
    return;
  }

  SDL_Surface *sur = TTF_RenderText_Solid(text->font, content, text->color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }

  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->texture = SDL_CreateTextureFromSurface(renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  free(text->content);
  text_content_update(text, content);

  SDL_FreeSurface(sur);
}

void text_change_color(struct text *text, SDL_Color color) {
  if (text == NULL) {
    return;
  }

  SDL_Surface *sur = TTF_RenderText_Solid(text->font, text->content, color);

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

static inline void text_content_update(struct text *text, char const *content) {
  size_t const len = strlen(content) + 1;
  text->content = malloc(sizeof(char) * len);
  strncpy(text->content, content, len);
  text->content[len] = '\0';
}
