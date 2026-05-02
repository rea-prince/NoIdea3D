#ifndef TEXTURE_CPP
#define TEXTURE_CPP

#include <SDL.h>
#include <SDL_image.h>
#include "texture.h"

texWrapper
createTexture(SDL_Renderer* renderer) {
    texWrapper tempTex;

    tempTex.texture = SDL_CreateTexture(
        renderer,
        MY_PIXEL_FORMAT,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH_C, HEIGHT_C
    );
    tempTex.format = SDL_AllocFormat(MY_PIXEL_FORMAT);

    return tempTex;
}
void
freeTexture(texWrapper* myTex) {
    SDL_DestroyTexture(myTex->texture);
    SDL_FreeFormat(myTex->format);
}

#endif
