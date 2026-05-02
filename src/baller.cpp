#ifndef BALLER_CPP
#define BALLER_CPP

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>

#include "texture.h"
#include "colors.h"
#include "vec.h"
#include "objects.h"

#define WIDTH_V  1.0f
#define HEIGHT_V 1.0f
#define DISTANCE 1.0f

/* NECESSARY FUNCTIONS */


/* Places a pixel at location x, y with respect to the Canvas dimensions
 *
 * @param x Horizontal coordinate
 * @param y Vertical coordinate
 * @param color Color to be assigned to the pixel
 * @param pitch Number of pixels per row
 * @return void
 */
static inline void
putPixel(int x, int y, uint32_t color, void* pixels, int pitch) {

  /* TRANSLATE COORDINATES */

  x += (WIDTH_C  / 2);
  y += (HEIGHT_C / 2);

  if (x < 0 || x >= WIDTH_C || y < 0 || y >= HEIGHT_C) {
    return;
  }

  /* DRAW TO TEXTURE */

  uint32_t* targetPixel = (uint32_t*)((uint8_t*) pixels + (y * pitch) + x * sizeof(uint32_t));
  *targetPixel = color;
}


/* Converts an x and y coordinate from pixels to a float or an arbitrary
 * unit equivalent to the units used by the viewport
 *
 * @param x Horizontal coordinate
 * @param y Vertical coordinate
 * @return Vec3d A 3d vector based on the canvas starting from the origin
 */
static inline Vec3d
canvasToViewport(int x, int y) {
  return (Vec3d) {
    .x = x * (WIDTH_V / WIDTH_C),
    .y = -y * (HEIGHT_V / HEIGHT_C),
    .z = DISTANCE
  };
}


/* Solves for points where the ray and sphere intersect using the equation
 * t^2 <vD, vD> + t (2<vCO, vD>) + <vCO, vCO> - r^2 = 0
 *
 * @param origin Location of the camera (where the ray originates)
 * @param vpVec Vector coming from the origin
 * @param sphere Contents of the sphere (center, radius, color)
 * @return Pair Points where the ray intersects with the sphere
 */
static Pair
intersectRaySphere(Point3d origin, Vec3d vpVec, Sphere sphere) {

  Vec3d co = subtractVec(origin, sphere.center);

  float a = dotVec(vpVec, vpVec);
  float b = 2 * dotVec(co, vpVec);
  float c = dotVec(co, co) - (sphere.radius * sphere.radius);

  float discriminant = b*b - 4*a*c;
  if (discriminant < 0) {
    return (Pair) {
      INFINITY, INFINITY
    };
  }

  return (Pair) {
    .t1 = (-b + (float)sqrt(discriminant)) / (2*a),
    .t2 = (-b - (float)sqrt(discriminant)) / (2*a),
  };
}


/* Computes for the total intensity of light at a specific point in the scene
 *
 * @param scene Pointer to the current scene
 * @param Point3d Point to determine the light intensity of
 * @param normal The direction of the normal line at the given point
 * @return float Total intensity of light at the given point
 */
static float
computeLighting(Scene* scene, Point3d point, Vec3d normal) {
  float  intensity = 0.0, dotted;
  Light* lights    = scene->lights;
  Vec3d  reflect;

  for (int i = 0; i < scene->numLights; i++) {
    if (lights[i].type == AMBIENT) {
      intensity += lights[i].intensity;
    } else {
      if (lights[i].type == POINT) {
        reflect = subtractVec(lights[i].position, point);
      } else {
        reflect = lights[i].direction;
      }

      dotted = dotVec(normal, reflect);

      if (dotted > 0) {
        intensity += (
          lights[i].intensity * dotted /
          (sqrt(dotVec(normal, normal)) * sqrt(dotVec(reflect, reflect)))
        );
      }
    }
  }
  return intensity;
}


/* Finds the closest intersecting point from any of the spheres in the scene
 * and returns its color
 *
 * @param scene Pointer to the current scene
 * @param origin Location of the camera (where the ray originates)
 * @param vpVec Vector coming from the origin
 * @param min Minimum value of the closest point
 * @param max Maximum value of the closest point
 * @return uint32_t Color of the closest point that intersect
 */
static uint32_t
traceRaySphere(Scene* scene, Point3d origin, Vec3d vpVec, float min, float max) {
  float  closestT      = max;
  Sphere closestSphere = {0};

  Vec3d   normal;
  Point3d point;

  for (int i = 0; i < scene->numSpheres; i++) {
    Pair solutions = intersectRaySphere(origin, vpVec, scene->spheres[i]);

    if (solutions.t1 >= min && solutions.t1 < max && solutions.t1 < closestT) {
      closestT = solutions.t1;
      closestSphere = scene->spheres[i];
    }
    if (solutions.t2 >= min && solutions.t2 < max && solutions.t2 < closestT) {
      closestT = solutions.t2;
      closestSphere = scene->spheres[i];
    }
  }

  if (closestSphere.color == 0) {
    return 0xFFFFFFFF;
  }

  point = addVec(origin, scalarProdVec(vpVec, closestT)); // intersection
  normal = normalizeVec(subtractVec(point, closestSphere.center));
  setLuminosity(&closestSphere.color, computeLighting(scene, point, normal));

  return closestSphere.color;
}


/* DRAWING FUNCTIONS */

void
drawBall(texWrapper* myTex, Scene* myScene) {

  void* pixels;
  int   pitch;

  SDL_LockTexture(myTex->texture, NULL, &pixels, &pitch);

  static float scaleX = WIDTH_V / WIDTH_C;
  static float scaleY = HEIGHT_V / HEIGHT_C;

  // TODO : Make origin adjustable via ImGui (wrap it in one of the structs)

  static Point3d origin = {0};
  Vec3d  vpVec;
  uint32_t color;

  /* DRAW IMAGE */

  for (int y = -HEIGHT_C / 2; y < HEIGHT_C / 2; y++) {
    for (int x = -WIDTH_C / 2; x < WIDTH_C / 2; x++) {
      vpVec = {
        x * scaleX,
        -y * scaleY,
        DISTANCE
      }; // vector from the camera (origin) to viewport

      color = traceRaySphere(myScene, origin, vpVec, 1, INFINITY);
      putPixel(x, y, color, pixels, pitch);
    }
  }

  /* --------- */

  SDL_UnlockTexture(myTex->texture);
}

#endif
