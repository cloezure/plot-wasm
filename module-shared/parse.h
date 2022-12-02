#pragma once

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
char* init_new_data_str(char *base64, int len);
