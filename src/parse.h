#pragma once
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

char* base64_decode(char const* cipher);

uint32_t get_uint32(char const *data, size_t offset);
float get_float(char const *data, size_t offset);
char *slice(char const *str, char *res, size_t start, size_t end);
