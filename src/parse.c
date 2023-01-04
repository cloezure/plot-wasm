#include "parse.h"

float get_float(char const *data, size_t offset) {
  float res = 0.0f;
  memcpy(&res, data + offset, sizeof(res));
  return res;
}

char *slice(char const *str, char *res, size_t start, size_t end) {
  return strncpy(res, str + start, end - start);
}
