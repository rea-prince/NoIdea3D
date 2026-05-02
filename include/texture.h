#ifndef TEXTURE_H
#define TEXTURE_H

#define WIDTH_C 800
#define HEIGHT_C 800
#define MY_PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

typedef struct texWrapper {
    SDL_Texture*     texture;
    SDL_PixelFormat* format;
} texWrapper;

texWrapper createTexture(SDL_Renderer* renderer);

void freeTexture(texWrapper* myTex);

#endif
