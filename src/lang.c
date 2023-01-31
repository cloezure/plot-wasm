#include "lang.h"
#include "comfun.h"
#include <stdarg.h>
#include <string.h>
#include <uchar.h>

struct lang *lang_now(struct graphics *graphics) {
  if (compare_str(graphics->lang, "ru")) {
    return lang_ru();
  } else if (compare_str(graphics->lang, "en")) {
    return lang_en();
  } else {
    return lang_en();
  }
}

struct lang *lang_init(char16_t const **data) {
  struct lang *lang = malloc(sizeof *lang);
  lang->info = malloc(sizeof(char16_t const *) * LANG_COUNT);
  for (size_t i = 0; i < LANG_COUNT; ++i) {
    lang->info[i] = malloc(strlen16(data[i]) * sizeof(char16_t) + 1);
    strcpy16(lang->info[i], data[i]);
  }
  return lang;
}

struct lang *lang_en(void) {
  char16_t const *data[] = {u"no data", u"click to open", u" channel off"};
  return lang_init(data);
}

struct lang *lang_ru(void) {
  char16_t const *data[] = {u"нет данных", u"нажмите, чтобы открыть",
                            u"канал выключен"};
  return lang_init(data);
}

void lang_free(struct lang *lang) {
  for (size_t i = 0; i < LANG_COUNT; ++i) {
    free(lang->info[i]);
  }

  free(lang);
}
