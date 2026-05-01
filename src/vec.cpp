#ifndef VEC_CPP
#define VEC_CPP

#include <math.h>

#include "vec.h"

/* DOT PRODUCT */

float dotVec(Vec3d a, Vec3d b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

/* SUBTRACT */

Vec3d subtractVec(Vec3d a, Vec3d b) {
    return (Vec3d) {
        .x = (a.x - b.x),
        .y = (a.y - b.y),
        .z = (a.z - b.z)
    };
}

/* ADD */

Vec3d addVec(Vec3d a, Vec3d b) {
    return (Vec3d) {
        .x = (a.x + b.x),
        .y = (a.y + b.y),
        .z = (a.z + b.z)
    };
}


#endif
