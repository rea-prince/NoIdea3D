#include <iostream>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <SDL.h>
#include <SDL_image.h>

const int WIDTH = 800, HEIGHT = 800;

int main(int argc, char *argv[]) {

    /* INITIALIZATION */

    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window *window_ptr = SDL_CreateWindow(
        "My renderer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 800,
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

    SDL_Event windowEvent; // input
    bool running = true;
    SDL_SetRenderDrawColor( // background color (rgba)
        renderer_ptr,
        120, 180, 255,
        255
    );

    /* WINDOW LOOP */
    while (running) {
        while (SDL_PollEvent (&windowEvent)) {
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

        // ***rendering here renders behind imgui***

        // draw imgui AFTER render clear BEFORE render present
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_ptr);

        // ***rendering here renders in front of imgui***

        SDL_RenderPresent(renderer_ptr);
    }

    /* CLOSE IMGUI */
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    /* DESTROY WINDOW */
    SDL_DestroyWindow(window_ptr);
    SDL_Quit();
    return EXIT_SUCCESS;
}
