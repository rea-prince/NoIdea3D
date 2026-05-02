#ifndef COLORS_CPP
#define COLORS_CPP

#include <SDL.h>

static uint8_t clamp(int value) {
    if (value > 255) return 255;
    else return value;
}

// this assumes the colors fit in 32 bits :))

void
setLuminosity(uint32_t* myColor, float intensity) {
    uint8_t r = ((*myColor >> 24) & 0xFF) * intensity;
    uint8_t g = ((*myColor >> 16) & 0xFF) * intensity;
    uint8_t b = ((*myColor >> 8) & 0xFF) * intensity;
    uint8_t a = *myColor;

    *myColor = ((uint32_t)r << 24) + ((uint32_t)g << 16) + ((uint32_t)b << 8) + ((uint32_t) a);
}

uint32_t
addColors(uint32_t myColor1, uint32_t myColor2) {

    uint32_t r = clamp(((myColor1 >> 24) & 0xFF) + ((myColor2 >> 24) & 0xFF)) << 24;
    uint32_t g = clamp(((myColor1 >> 16) & 0xFF) + ((myColor2 >> 16) & 0xFF)) << 16;
    uint32_t b = clamp(((myColor1 >> 8) & 0xFF) + ((myColor2 >> 8) & 0xFF)) << 8;
    uint32_t a = clamp(((myColor1 & 0xFF)) + ((myColor2 & 0xFF)));

    return r | g | b | a;
}

#endif
