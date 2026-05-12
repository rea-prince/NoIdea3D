#ifndef COLORS_CPP
#define COLORS_CPP

#include <SDL.h>

uint8_t clamp(float value) {
  if (value < 0.0f) return 0;
  if (value > 255.0f) return 255;

  return value;
}

// this assumes the colors fit in 32 bits :))

void
setLuminosity(uint32_t* myColor, float intensity) {
  float r = ((*myColor >> 24) & 0xFF) * intensity;
  float g = ((*myColor >> 16) & 0xFF) * intensity;
  float b = ((*myColor >> 8)  & 0xFF) * intensity;

  uint8_t a = *myColor;

  *myColor = (
    ((uint32_t) clamp(r) << 24) |
    ((uint32_t) clamp(g) << 16) |
    ((uint32_t) clamp(b) << 8)  |
    a
  );
}

uint32_t
addColors(uint32_t myColor1, uint32_t myColor2) {

  uint32_t r = clamp(
                 ((myColor1 >> 24) & 0xFF) + ((myColor2 >> 24) & 0xFF)
               ) << 24;
  uint32_t g = clamp(
                ((myColor1 >> 16) & 0xFF) + ((myColor2 >> 16) & 0xFF)
               ) << 16;
  uint32_t b = clamp(
                ((myColor1 >> 8) & 0xFF) + ((myColor2 >> 8) & 0xFF)
               ) << 8;
  uint32_t a = clamp(
                (myColor1 & 0xFF) + (myColor2 & 0xFF)
               );

  return r | g | b | a;
}

#endif
