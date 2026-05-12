#ifndef BALLER_CPP
#define BALLER_CPP

#include <stdio.h>
#include <string.h>
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
 * @param  x     Horizontal coordinate
 * @param  y     Vertical coordinate
 * @param  color Color to be assigned to the pixel
 * @param  pitch Number of pixels per row
 * @return void
 */
static inline void
putPixel(int x, int y, uint32_t color, void *pixels, int pitch) {

  /* TRANSLATE COORDINATES */

  x += (WIDTH_C  / 2);
  y += (HEIGHT_C / 2);

  if (x < 0 || x >= WIDTH_C || y < 0 || y >= HEIGHT_C) {
    return;
  }

  /* DRAW TO TEXTURE */

  uint32_t* targetPixel = (uint32_t*)((uint8_t*) pixels +
                          (y * pitch) + x * sizeof(uint32_t));
  *targetPixel = color;
}


/* Converts an x and y coordinate from pixels to a float or an arbitrary
 * unit equivalent to the units used by the viewport
 *
 * @param  x     Horizontal coordinate
 * @param  y     Vertical coordinate
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

static inline Vec3d
reflectRay(Vec3d normal, Vec3d reflect) {
  return subtractVec(
    scalarProdVec(normal, 2 * dotVec(normal, reflect)),
    reflect
  );
}

/* Solves for points where the ray and sphere intersect using the equation
 * t^2 <vD, vD> + t (2<vCO, vD>) + <vCO, vCO> - r^2 = 0
 *
 * @param  origin Location of the camera (where the ray originates)
 * @param  direc  Vector of the ray
 * @param  sphere Contents of the sphere (center, radius, color)
 * @return Pair   Points where the ray intersects with the sphere
 */
static Pair
intersectRaySphere(Point3d origin, Vec3d direc, Sphere sphere) {

  Vec3d co = subtractVec(origin, sphere.center);

  float a = dotVec(direc, direc);
  float b = 2 * dotVec(co, direc);
  float c = dotVec(co, co) - (sphere.radius * sphere.radius);

  float discriminant = b * b - 4 * a * c;

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

/* Finds the closest intersecting sphere given a viewport vector
 *
 * @param  scene    Pointer to the current scene
 * @param  origin   Origin of the ray
 * @param  direc    Vector of the ray
 * @param  min      Minimum value of the closest point
 * @param  max      Maximum value of the closest point
 * @return closestT Pointer to a variable that will store the distance
 *                  of the closest point
 */
static Sphere
closestIntersection(Scene *scene, Point3d origin, Vec3d direc,
                    float min, float max, float *closestT) {

  Sphere closestSphere = {0};
  Pair   solutions;

  // find closest intersecting points

  for (int i = 0; i < scene->numSpheres; i++) {
    solutions = intersectRaySphere(origin, direc, scene->spheres[i]);

    if (solutions.t1 >= min && solutions.t1 <= max &&
        solutions.t1 < *closestT) {

      *closestT      = solutions.t1;
       closestSphere = scene->spheres[i];

    }
    if (solutions.t2 >= min && solutions.t2 <= max &&
        solutions.t2 < *closestT) {

      *closestT      = solutions.t2;
       closestSphere = scene->spheres[i];

    }
  }

  return closestSphere;
}


/* Computes for the total intensity of light at a specific point in the scene
 *
 * @param  scene      Pointer to the current scene
 * @param  Point3d    Point to determine the light intensity of
 * @param  normal     The direction of the normal line at the given point
 * @param  viewVector Vector coming from the viewport
 * @param  specular   Specular reflectivity of the object
 * @return float      Total intensity of light at the given point
 */
static float
computeLighting(Scene *scene, Point3d point, Vec3d normal,
                Vec3d viewVector, float specular) {

  float dottedNL;
  float dottedRV;
  float intensity = 0.0;
  float max;
  float shadowT = INFINITY;

  Light *lights = scene->lights;
  Vec3d  light;
  Vec3d  reflect;

  Sphere shadowSphere = {0};

  // go through all light sources

  for (int i = 0; i < scene->numLights; i++) {
    if (lights[i].type == AMBIENT) {
      intensity += lights[i].intensity;
    } else {
      if (lights[i].type == POINT) {
        light = subtractVec(lights[i].position, point);
        max = 1.0f;
      } else {
        light = lights[i].direction;
        max = INFINITY;
      }

      // check for shadows or intersections of light with another object

      shadowSphere = closestIntersection(
        scene,
        point, light,
        0.001f, max, &shadowT
      );
      if (shadowT < INFINITY) {
        continue;
      }

      // diffuse (light source intensity * diffusion ratio or I / A)

      dottedNL = dotVec(normal, light);
      if (dottedNL > 0) {
        intensity += (
          lights[i].intensity * dottedNL /
          (sqrt(dotVec(normal, normal)) * sqrt(dotVec(light, light)))
        );
      }

      // specular reflection

      if (specular != -1) {
        light = normalizeVec(light);

        // 2 * N * dot(N, L) - L

        reflect = reflectRay(normal, light);

        dottedRV = dotVec(reflect, viewVector);

        if (dottedRV > 0) {

          // light.intensity * pow(r_dot_v/(length(R) * length(V)), s)

          intensity += lights[i].intensity * pow(
                        dottedRV /
                        (sqrt(dotVec(reflect, reflect)) *
                          sqrt(dotVec(viewVector, viewVector))),
                        specular
                       );
        }

      }

    }
  }

  if (intensity >= 1.0f) {
    return 1.0f;
  }

  return intensity;
}

/* Finds the closest intersecting point from any of the spheres in the scene
 * and returns its color
 *
 * @param  scene    Pointer to the current scene
 * @param  origin   Origin of the ray
 * @param  direc    Vector coming from the origin
 * @param  min      Minimum value of the closest point
 * @param  max      Maximum value of the closest point
 * @return uint32_t Color of the closest point that intersect
 */
static uint32_t
traceRaySphere(Scene *scene, Point3d origin, Vec3d direc,
               float min, float max, int depth) {

  float  closestT      = max;
  Sphere closestSphere = closestIntersection(
                           scene, origin,
                           direc, min, max,
                           &closestT
                         );

  // return white if none

  if (closestSphere.color == 0) {
    return BACKGROUND_COLOR;
  }

  // compute for luminosity of pixel

  Point3d  point      = addVec(origin, scalarProdVec(direc, closestT));
  Vec3d    normal     = normalizeVec(subtractVec(point, closestSphere.center));
  Vec3d    viewVector = scalarProdVec(direc, -1.0f);
  uint32_t localColor = closestSphere.color;

  setLuminosity(
    &localColor,
    computeLighting(scene, point, normal,
                    invertVec(direc),
                    closestSphere.specular)
  );

  float reflectivity = closestSphere.reflective;

  // check for recursive limit

  if (depth <= 0 || reflectivity <= 0) {
    return localColor;
  }

  // recurse for reflectivity

  Vec3d reflectedRay = reflectRay(normal, invertVec(direc));

  uint32_t reflectedColor = traceRaySphere(
    scene, point, reflectedRay,
    0.04f, INFINITY, depth - 1
  );
  setLuminosity(&localColor, (1 - reflectivity));
  setLuminosity(&reflectedColor, reflectivity);


  return addColors(localColor, reflectedColor);
}


/* DRAWING FUNCTIONS */

void
drawBall(texWrapper *myTex, Scene *myScene) {

  void* pixels;
  int   pitch;

  SDL_LockTexture(myTex->texture, NULL, &pixels, &pitch);

  static float scaleX = WIDTH_V / WIDTH_C;
  static float scaleY = HEIGHT_V / HEIGHT_C;

  // TODO : Make origin adjustable via ImGui (wrap it in one of the structs)

  static Point3d origin = {0};
  Vec3d  vpVec;
  uint32_t color;
  int depth = 3;

  /* DRAW IMAGE */

  for (int y = -HEIGHT_C / 2; y < HEIGHT_C / 2; y++) {
    for (int x = -WIDTH_C / 2; x < WIDTH_C / 2; x++) {
      vpVec = {
        x * scaleX,
        -y * scaleY,
        DISTANCE
      }; // vector from the camera (origin) to viewport

      color = traceRaySphere(myScene, origin, vpVec, 1, INFINITY, depth);
      putPixel(x, y, color, pixels, pitch);
    }
  }

  /* --------- */

  SDL_UnlockTexture(myTex->texture);
}

#endif
