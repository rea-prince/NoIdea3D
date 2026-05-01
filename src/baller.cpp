#ifndef BALLER_CPP
#define BALLER_CPP

#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"
#include "colors.h"
#include "vec.h"

#define WIDTH_V  1.0
#define HEIGHT_V 1.0
#define DISTANCE 1.0

static void putPixel(int x, int y, int color, void* pixels, int pitch) {

    /* TRANSLATE COORDINATES */

    x += (WIDTH_C  / 2);
    y += (HEIGHT_C / 2);

    if (x < 0 || x >= WIDTH_C || y < 0 || y >= HEIGHT_C) {
        return;
    }

    /* DRAW TO TEXTURE */

    Uint32* targetPixel = (Uint32*)((Uint8*) pixels + y * pitch + x * sizeof(Uint32));
    *targetPixel = color;
}

static Vec3d canvasToViewport(int x, int y) {
    return (Vec3d) {
        .x = x * ((float) WIDTH_V / WIDTH_C),
        .y = y * ((float) HEIGHT_V / HEIGHT_C),
        .z = DISTANCE
    };
}

void drawBall(texWrapper* myTex) {

    Uint32 color = SDL_MapRGBA(myTex->format, 255, 255, 255, 255);
    void* pixels;
    int pitch;

    SDL_LockTexture(myTex->texture, NULL, &pixels, &pitch);

    /* DRAW HERE */

    Vec3d origin = {0};

    for (int x = -WIDTH_C / 2; x < WIDTH_C / 2; x++) {
        for (int y = -HEIGHT_C / 2; x < HEIGHT_C / 2; x++) {
            Vec3d dist = canvasToViewport(x, y); // vector from the camera (origin) to viewport

        }
    }


    putPixel(56, 5, color, pixels, pitch);

    /* --------- */

    SDL_UnlockTexture(myTex->texture);
}

#endif
