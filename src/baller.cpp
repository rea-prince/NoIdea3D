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
// #define INFINITY 99

/* NECESSARY FUNCTIONS */

static void putPixel(int x, int y, uint32_t color, void* pixels, int pitch) {

    /* TRANSLATE COORDINATES */

    x += (WIDTH_C  / 2);
    y += (HEIGHT_C / 2);

    if (x < 0 || x >= WIDTH_C || y < 0 || y >= HEIGHT_C) {
        return;
    }

    /* DRAW TO TEXTURE */

    uint32_t* targetPixel = (uint32_t*)((uint8_t*) pixels + y * pitch + x * sizeof(uint32_t));
    *targetPixel = color;
}

static Vec3d canvasToViewport(int x, int y) {
    return (Vec3d) {
        .x = x * (WIDTH_V / WIDTH_C),
        .y = -y * (HEIGHT_V / HEIGHT_C),
        .z = DISTANCE
    };
}

static Pair intersectRaySphere(Vec3d origin, Vec3d dest, Sphere sphere) {

    Vec3d co = subtractVec(origin, sphere.center);

    float a = dotVec(dest, dest);
    float b = 2 * dotVec(co, dest);
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

static uint32_t traceRay(Scene* scene, Vec3d origin, Vec3d dest, float minT, float maxT) {
    float closestT = maxT;
    Sphere closestSphere = {0};

    for (int i = 0; i < scene->numSpheres; i++) {
        Pair solutions = intersectRaySphere(origin, dest, scene->spheres[i]);
        if (solutions.t1 >= minT && solutions.t1 < maxT && solutions.t1 < closestT) {
            closestT = solutions.t1;
            closestSphere = scene->spheres[i];
        }
        if (solutions.t2 >= minT && solutions.t2 < maxT && solutions.t2 < closestT) {
            closestT = solutions.t2;
            closestSphere = scene->spheres[i];
        }
    }

    if (closestSphere.color == 0) {
        return 0xFFFFFFFF;
    }
    return closestSphere.color;
}

/* DRAWING FUNCTIONS */

void drawBall(texWrapper* myTex, Scene* myScene) {

    uint32_t color = SDL_MapRGBA(myTex->format, 255, 255, 255, 255);
    void* pixels;
    int pitch;

    SDL_LockTexture(myTex->texture, NULL, &pixels, &pitch);

    /* DRAW HERE */

    Vec3d origin = {0};

    for (int x = -WIDTH_C / 2; x < WIDTH_C / 2; x++) {
        for (int y = -HEIGHT_C / 2; y < HEIGHT_C / 2; y++) {
            Vec3d dest = canvasToViewport(x, y); // vector from the camera (origin) to viewport
            uint32_t color = traceRay(myScene, origin, dest, 1, INFINITY);
            putPixel(x, y, color, pixels, pitch);
        }
    }

    /* --------- */

    SDL_UnlockTexture(myTex->texture);
}

#endif
