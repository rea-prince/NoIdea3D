#ifndef BALLER_H
#define BALLER_H

#define WIDTH 800
#define HEIGHT 800

SDL_Color bgc = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 255,
};

void renderBall(SDL_Renderer* renderer_ptr);

#endif
