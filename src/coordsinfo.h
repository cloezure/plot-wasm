#pragma once
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

typedef struct CoordsInfo {
    TTF_Font    * font;
    SDL_Texture * texture;
    SDL_Color color;
    SDL_Rect  position;
} coordsinfo_t;

coordsinfo_t* CoordsInfo_init(char const* font_path,
                              int32_t font_size,
                              SDL_Color color,
                              SDL_Rect position);

void CoordsInfo_free(coordsinfo_t *cordinfo);

void CoordsInfo_update_coords(coordsinfo_t *ci, float x, float y);
