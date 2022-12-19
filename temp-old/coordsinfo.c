#include "coordsinfo.h"
#include "globalplotvar.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

coordsinfo_t* CoordsInfo_init(char const* font_path, int32_t font_size, SDL_Color color, SDL_Rect position) {
    coordsinfo_t * new_ci = malloc(sizeof(*new_ci));
    new_ci->font = TTF_OpenFont(font_path, font_size);

    if (new_ci->font == NULL) {
        DISPLAY_ERROR_TTF("font not load!");
    }

    new_ci->color = color;
    SDL_Surface * sur = TTF_RenderText_Solid(new_ci->font, "[x: 0; y: 0]", new_ci->color);
    if ( sur == NULL ) {
        DISPLAY_ERROR_SDL("surface can't init by RenderText");
    }

    position.w = sur->w;
    position.h = sur->h;
    new_ci->position = position;

    new_ci->texture = SDL_CreateTextureFromSurface(g_ren, sur);
    if ( new_ci->texture == NULL ) {
        DISPLAY_ERROR_SDL("can't init new_coords_info->textrure");
    }

    SDL_FreeSurface(sur);

    return new_ci;
}

void CoordsInfo_update_coords(coordsinfo_t * ci, float x, float y) {
    assert(ci != NULL);
    assert(ci->font != NULL);

    char coords_info_buff[100];
    sprintf(coords_info_buff, "[x: %.2f; y: %.2f]", x, y);
    SDL_Surface * sur = TTF_RenderText_Blended(ci->font, coords_info_buff, ci->color);

    if ( sur == NULL ) {
        DISPLAY_ERROR_SDL("surface can't update coords");
    }

    assert(ci->texture != NULL);
    SDL_DestroyTexture(ci->texture);
    ci->texture = NULL;

    ci->texture = SDL_CreateTextureFromSurface(g_ren, sur);
    if ( ci->texture == NULL ) {
        DISPLAY_ERROR_SDL("can't init new_coords_info->textrure");
    }

    SDL_FreeSurface(sur);
}

void CoordsInfo_free(coordsinfo_t *ci) {
    assert(ci != NULL);
    assert(ci->font != NULL);
    TTF_CloseFont(ci->font);
    ci->font = NULL;

    SDL_DestroyTexture(ci->texture);
    ci->texture = NULL;

    free(ci);
    ci = NULL;
}
