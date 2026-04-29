#ifndef BALLER_H
#define BALLER_H

#define WIDTH 800
#define HEIGHT 800
#define MY_PIXEL_FORMAT SDL_PIXELFORMAT_RGBA8888

typedef struct texWrapper {
    SDL_Texture* texture;
    SDL_PixelFormat* format;
} texWrapper;


texWrapper createTexture(SDL_Renderer* renderer);
void freeTexture(texWrapper* myTex);

void drawBall(texWrapper* myTex);

#endif
