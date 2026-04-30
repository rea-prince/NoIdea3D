#ifndef COLORS_CPP
#define COLORS_CPP

#include <SDL.h>

Uint8 clamp(int value) {
    if (value > 255) return 255;
    else return value;
}

// this assumes the colors fit in 32 bits :))

void setLuminosity(Uint32* myColor, int intensity) {
    Uint8 r = ((*myColor >> 24) & 0xFF) * intensity;
    Uint8 g = ((*myColor >> 16) & 0xFF) * intensity;
    Uint8 b = ((*myColor >> 8) & 0xFF) * intensity;
    Uint8 a = *myColor;

    *myColor = ((Uint32)r << 6) + ((Uint32)g << 4) + ((Uint32)b << 2) + (Uint32) a);
}

Uint32 addColors32(Uint32 myColor1, Uint32 myColor2) {

    Uint32 r = clamp(((myColor1 >> 24) & 0xFF) + ((myColor2 >> 24) & 0xFF)) << 24;
    Uint32 g = clamp(((myColor1 >> 16) & 0xFF) + ((myColor2 >> 16) & 0xFF)) << 16;
    Uint32 b = clamp(((myColor1 >> 8) & 0xFF) + ((myColor2 >> 8) & 0xFF)) << 8;
    Uint32 a = clamp(((myColor1 & 0xFF)) + ((myColor2 & 0xFF)));

    return r | g | b | a;
}

#endif
