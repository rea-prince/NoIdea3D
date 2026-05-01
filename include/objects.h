#ifndef OBJECTS_H
#define OBJECTS_H

#define MAX_SPHERES 3

#include <SDL.h>
#include "vec.h"

typedef struct Sphere {
    Point3d center;
    float radius;
    uint32_t color;
} Sphere;

typedef struct Scene {
    Sphere spheres[MAX_SPHERES];
    int numSpheres;
} Scene;


#endif
