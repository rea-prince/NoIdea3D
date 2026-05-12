#ifndef VEC_CPP
#define VEC_CPP

#include <math.h>

#include "vec.h"

/* NORMALIZE */

Vec3d
normalizeVec(Vec3d vec) {
  float magnitude = sqrt(dotVec(vec, vec));
  if (magnitude > 0.0f) {
    return (Vec3d) {
      .x = vec.x / magnitude,
      .y = vec.y / magnitude,
      .z = vec.z / magnitude
    };
  }
  return (Vec3d) {0};
}

/* INVERT */

Vec3d
invertVec(Vec3d vec) {
  return (Vec3d) {
    .x = -vec.x,
    .y = -vec.y,
    .z = -vec.z
  };
}

/* SCALAR PRODUCT */

Vec3d
scalarProdVec(Vec3d vec, float multiplier) {
  return (Vec3d) {
    .x = vec.x * multiplier,
    .y = vec.y * multiplier,
    .z = vec.z * multiplier
  };
}

/* DOT PRODUCT */

float
dotVec(Vec3d a, Vec3d b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/* SUBTRACT */

Vec3d
subtractVec(Vec3d a, Vec3d b) {
  return (Vec3d) {
    .x = (a.x - b.x),
    .y = (a.y - b.y),
    .z = (a.z - b.z)
  };
}

/* ADD */

Vec3d
addVec(Vec3d a, Vec3d b) {
  return (Vec3d) {
    .x = (a.x + b.x),
    .y = (a.y + b.y),
    .z = (a.z + b.z)
  };
}


#endif
