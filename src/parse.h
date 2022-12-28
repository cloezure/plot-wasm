#pragma once
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>


#define GET_UINT32_BE(n,b,i)                            \
    {                                                   \
        (n) = ( (uint32_t) (b)[(i)    ] << 24 )         \
            | ( (uint32_t) (b)[(i) + 1] << 16 )         \
            | ( (uint32_t) (b)[(i) + 2] <<  8 )         \
            | ( (uint32_t) (b)[(i) + 3]       );        \
    }

char* base64_decode(char const* cipher);
float get_float(char const *data, size_t offset);
char *slice(char const *str, char *res, size_t start, size_t end);
