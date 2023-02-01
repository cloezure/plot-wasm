#pragma once

#include "graphics.h"
#include <uchar.h>

enum {
  NO_DATA = 0,
  CLICK_TO_OPEN = 1,
  CHANNEL_OFF = 2,
};

#define LANG_COUNT 3
struct lang {
  char16_t **info;
};

struct lang *lang_en(void);
struct lang *lang_ru(void);
void lang_free(struct lang *lang);

void change_lang(struct lang **lang, char const *locale);
