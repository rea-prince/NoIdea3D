#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL.h>
#include "vec.h"

#define MAX_SPHERES 4
#define MAX_LIGHTING 4

enum LightType {
  AMBIENT     = 0,
  POINT       = 1,
  DIRECTIONAL = 2
};

typedef struct Sphere {
  Point3d  center;
  float    radius;
  uint32_t color;
  float specular;
} Sphere;

typedef struct Light {
  Point3d   position;
  Vec3d     direction;
  LightType type;
  float     intensity;
} Light;

typedef struct Scene {
  Sphere spheres[MAX_SPHERES];
  Light  lights[MAX_LIGHTING];
  int    numLights;
  int    numSpheres;
} Scene;


#endif
