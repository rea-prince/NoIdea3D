#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

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

  // IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui_ImplSDL2_InitForSDLRenderer(
    window_ptr,
    renderer_ptr
  );
  ImGui_ImplSDLRenderer2_Init(renderer_ptr);


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
    .specular = 500
  };
  myScene.spheres[1] = {
    .center = {2, 0, 4},
    .radius = 1,
    .color  = SDL_MapRGBA(myTex.format, 0, 0, 255, 255),
    .specular = 500
  };
  myScene.spheres[2] = {
    .center = {-2, 0, 4},
    .radius = 1,
    .color  = SDL_MapRGBA(myTex.format, 0, 255, 0, 255),
    .specular = 500
  };
  myScene.spheres[3] = {
    .center = {0, -5001, 0},
    .radius = 5000,
    .color  = SDL_MapRGBA(myTex.format, 255, 255, 0, 255),
    .specular = 500
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
      ImGui_ImplSDL2_ProcessEvent(&windowEvent); // ImGui input
    }

    /* Create ImGui frame */

    ImGui_ImplSDL2_NewFrame();
    ImGui_ImplSDLRenderer2_NewFrame();

    ImGui::NewFrame();

    ImGui::Begin("test");
    ImGui::Text("Put buttons here\nPut buttons here\nPut buttons here\n");
    ImGui::End();

    ImGui::Render();

    SDL_RenderClear(renderer_ptr); // clear renderer for imgui

    /* ------------------------------------------- */

    SDL_RenderCopy(renderer_ptr, myTex.texture, NULL, NULL);

    /* ------------------------------------------- */

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_ptr);

    SDL_RenderPresent(renderer_ptr);
  }

  /* CLOSE IMGUI */

  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  /* FREE TEXTURE */

  freeTexture(&myTex);

  /* DESTROY WINDOW */

  SDL_DestroyWindow(window_ptr);
  SDL_Quit();
  return EXIT_SUCCESS;
}
