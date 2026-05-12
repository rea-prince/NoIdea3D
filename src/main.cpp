#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"
#include "objects.h"

#include "baller.h"

int
main(int argc, char *argv[]) {

  /* INITIALIZATION */

  SDL_Init( SDL_INIT_EVERYTHING );
  SDL_Window *window_ptr = SDL_CreateWindow(
    "My renderer",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WIDTH_C, HEIGHT_C,
    SDL_WINDOW_SHOWN
  );
  if (!window_ptr) {
    SDL_Quit();
    return -1;
  }
  SDL_Renderer *renderer_ptr = SDL_CreateRenderer(window_ptr, -1, 0);
  if (!renderer_ptr) {
    SDL_DestroyWindow(window_ptr);
    SDL_Quit();
    return -1;
  }
  SDL_SetRenderTarget(renderer_ptr, NULL);


  /* ------------------------------------------- */

  texWrapper myTex = createTexture(renderer_ptr);

  /* SCENE SETUP */

  Scene myScene = {
    .numLights  = 3,
    .numSpheres = 4
  };

  // TODO : Make lighting adjustable via ImGui

  myScene.lights[0] = {
    .type    = AMBIENT,
    .intensity = 0.2
  };
  myScene.lights[1] = {
    .position  = {2, 1, 0},
    .type    = POINT,
    .intensity = 0.6
  };
  myScene.lights[2] = {
    .direction = {1, 4, 4},
    .type    = DIRECTIONAL,
    .intensity = 0.2
  };

  // TODO : Make objects adjustable via ImGui

  myScene.spheres[0] = {
    .center = {0, -1, 3},
    .radius = 1,
    .color  = SDL_MapRGBA(myTex.format, 255, 0, 0, 255),
    .specular = 500,
    .reflective = 0.2
  };
  myScene.spheres[1] = {
    .center = {2, 0, 4},
    .radius = 1,
    .color  = SDL_MapRGBA(myTex.format, 0, 0, 255, 255),
    .specular = 500,
    .reflective = 0.3
  };
  myScene.spheres[2] = {
    .center = {-2, 0, 4},
    .radius = 1,
    .color  = SDL_MapRGBA(myTex.format, 0, 255, 0, 255),
    .specular = 10,
    .reflective = 0.4
  };
  myScene.spheres[3] = {
    .center = {0, -5001, 0},
    .radius = 5000,
    .color  = SDL_MapRGBA(myTex.format, 255, 255, 0, 255),
    .specular = 1000,
    .reflective = 0.5
  };
  drawBall(&myTex, &myScene);

  /* WINDOW LOOP */

  SDL_Event windowEvent; // input
  bool running = true;

  while(running) {
    while (SDL_PollEvent(&windowEvent)) {
      if (SDL_QUIT == windowEvent.type) {
        running = false;
      }
    }

    SDL_RenderClear(renderer_ptr); // clear renderer for imgui

    /* ------------------------------------------- */

    SDL_RenderCopy(renderer_ptr, myTex.texture, NULL, NULL);

    /* ------------------------------------------- */

    SDL_RenderPresent(renderer_ptr);
  }

  /* FREE TEXTURE */

  freeTexture(&myTex);

  /* DESTROY WINDOW */

  SDL_DestroyWindow(window_ptr);
  SDL_Quit();
  return EXIT_SUCCESS;
}
