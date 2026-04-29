#ifndef BALLER_CPP
#define BALLER_CPP

#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include "baller.h"

static void putPixel(uint16_t x, uint16_t y, uint32_t color, void* pixels, uint32_t pitch) {

    /* TRANSLATE COORDINATES */

    x += (WIDTH  / 2);
    y += (HEIGHT / 2);

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return;
    }

    /* DRAW TO TEXTURE */

    uint32_t* targetPixel = (uint32_t*)((uint8_t*)pixels + y * pitch + x * sizeof(uint32_t));
        *targetPixel = pixelColor;
}

void renderBall(SDL_Renderer* renderer_ptr) {
    SDL_Texture *myTex = SDL_CreateTexture(
        renderer_ptr,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    void* pixels;
    int pitch = WIDTH * 32;

    SDL_LockTexture(myTex, NULL, &pixels, &pitch);



    SDL_UnlockTexture(myTex);

}

#endif
