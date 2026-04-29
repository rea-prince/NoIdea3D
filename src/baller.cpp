#ifndef BALLER_CPP
#define BALLER_CPP

#include <SDL.h>
#include <SDL_image.h>
#include "texture.h"

static void putPixel(Uint16 x, Uint16 y, Uint32 color, void* pixels, Uint32 pitch) {

    /* TRANSLATE COORDINATES */

    x += (WIDTH  / 2);
    y += (HEIGHT / 2);

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }

    /* DRAW TO TEXTURE */

    Uint32* targetPixel = (Uint32*)((Uint8*)pixels + y * pitch + x * sizeof(Uint32));
    *targetPixel = color;
}

void drawBall(texWrapper* myTex) {

    Uint32 color = SDL_MapRGBA(myTex->format, 255, 255, 255, 255);
    void* pixels;
    int pitch;

    SDL_LockTexture(myTex->texture, NULL, &pixels, &pitch);

    putPixel(56, 5, color, pixels, pitch);

    SDL_UnlockTexture(myTex->texture);
}

#endif
