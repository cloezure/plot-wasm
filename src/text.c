#include "text.h"
#include "common_functions.h"
#include "global_vars.h"
#include <assert.h>
#include <string.h>

static inline void text16_content_update(struct text16 *text,
                                         char16_t const *content);

static inline void text8_content_update(struct text8 *text,
                                        char const *content);

#define TEXT_FREE(TEXT)                                                        \
  assert(text);                                                                \
  TTF_CloseFont(text->font);                                                   \
  text->font = NULL;                                                           \
  SDL_DestroyTexture(text->texture);                                           \
  text->texture = NULL;                                                        \
  free(text->content);                                                         \
  text->content = NULL;                                                        \
  free(text)

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

struct text16 *text16_init(char const *font_path, int32_t font_size,
                           SDL_Color color, SDL_Point position,
                           char16_t const *content) {
  struct text16 *new_text = malloc(sizeof *new_text);
  new_text->font = TTF_OpenFont(font_path, font_size);

  if (new_text->font == NULL) {
    display_error_ttf("font not load");
  }

  new_text->color = color;
  SDL_Surface *sur =
      TTF_RenderUNICODE_Blended(new_text->font, content, new_text->color);
  if (sur == NULL) {
    display_error_sdl("surface could not be created");
  }

  new_text->position.x = position.x;
  new_text->position.y = position.y;
  new_text->position.w = sur->w;
  new_text->position.h = sur->h;

  new_text->texture = SDL_CreateTextureFromSurface(g_renderer, sur);
  if (new_text->texture == NULL) {
    display_error_sdl("texture could not be create by surface");
  }

  SDL_FreeSurface(sur);

  text16_content_update(new_text, content);
  return new_text;
}

void text16_free(struct text16 *text) { TEXT_FREE(text); }

void text16_change_content(struct text16 *text, char16_t const *content) {
  SDL_Surface *sur =
      TTF_RenderUNICODE_Blended(text->font, content, text->color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }

  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->texture = SDL_CreateTextureFromSurface(g_renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  free(text->content);
  text16_content_update(text, content);

  SDL_FreeSurface(sur);
}

void text16_change_color(struct text16 *text, SDL_Color color) {
  SDL_Surface *sur =
      TTF_RenderUNICODE_Blended(text->font, text->content, color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }
  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->position.w = sur->w;
  text->position.h = sur->h;

  text->texture = SDL_CreateTextureFromSurface(g_renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  SDL_FreeSurface(sur);
}

static inline void text16_content_update(struct text16 *text,
                                         char16_t const *content) {
  size_t const len = strlen16(content) + 1;
  text->content = malloc(sizeof(char16_t) * len);
  strcpy16(text->content, content);
  text->content[len] = u'\0';
}

static inline void text8_content_update(struct text8 *text,
                                        char const *content) {
  size_t const len = strlen(content) + 1;
  text->content = malloc(sizeof(char) * len);
  strcpy(text->content, content);
  text->content[len] = '\0';
}

struct text8 *text8_init(char const *font_path, int32_t font_size,
                         SDL_Color color, SDL_Point position,
                         char const *content) {
  struct text8 *new_text = malloc(sizeof *new_text);
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

  new_text->texture = SDL_CreateTextureFromSurface(g_renderer, sur);
  if (new_text->texture == NULL) {
    display_error_sdl("texture could not be create by surface");
  }

  SDL_FreeSurface(sur);

  text8_content_update(new_text, content);
  return new_text;
}

void text8_free(struct text8 *text) { TEXT_FREE(text); }

void text8_change_content(struct text8 *text, char const *content) {
  SDL_Surface *sur = TTF_RenderText_Blended(text->font, content, text->color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }

  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->texture = SDL_CreateTextureFromSurface(g_renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  free(text->content);
  text8_content_update(text, content);
  SDL_FreeSurface(sur);
}

void text8_change_color(struct text8 *text, SDL_Color color) {
  SDL_Surface *sur = TTF_RenderText_Blended(text->font, text->content, color);

  if (sur == NULL) {
    display_error_sdl("surface in text could not be init");
  }
  SDL_DestroyTexture(text->texture);
  text->texture = NULL;

  text->position.w = sur->w;
  text->position.h = sur->h;

  text->texture = SDL_CreateTextureFromSurface(g_renderer, sur);
  if (text->texture == NULL) {
    display_error_sdl("text->texture could not be init");
  }

  SDL_FreeSurface(sur);
}
