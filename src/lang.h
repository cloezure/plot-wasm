#pragma once

#include "graphics.h"
#include <uchar.h>

enum {
  NO_DATA = 0,
  CLICK_TO_OPEN = 1,
  CHANNEL_OFF = 2,
};

enum LOCAL_LANG {
  LOCAL_LANG_RU = 0,
  LOCAL_LANG_EN = 1,
};

#define LANG_COUNT 3
struct lang {
  char16_t **info;
};

struct lang *lang_en(void);
struct lang *lang_ru(void);

struct lang *lang_now(struct graphics *graphics);
void lang_free(struct lang *lang);
