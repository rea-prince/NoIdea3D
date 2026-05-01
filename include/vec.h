#ifndef VEC_H
#define VEC_H

typedef struct Vec3d {
    float x;
    float y;
    float z;
} Vec3d;

typedef struct Vec {
    float t1;
    float t2;
} Pair;

float dotVec(Vec3d a, Vec3d b);
Vec3d subtractVec(Vec3d a, Vec3d b);
Vec3d addVec(Vec3d a, Vec3d b);

#endif
