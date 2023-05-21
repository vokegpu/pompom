#include "ekg/ekg.hpp"

int32_t main(int32_t, char**) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_DisplayMode sdldisplaymode {};
    SDL_GetDisplayMode(0, 0, &sdldisplaymode);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

    SDL_Window *psdlwin {SDL_CreateWindow("pompom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdldisplaymode.w, sdldisplaymode.h, SDL_WINDOW_OPENGL)};
    SDL_GLContext sdlglcontext {SDL_GL_CreateContext(psdlwin)};
    SDL_Event sdlevent {};

    bool running {true};
    uint32_t frameratethreadelay {1000 / 60};

    glViewport(0, 0, sdldisplaymode.w, sdldisplaymode.h);
    SDL_GL_MakeCurrent(psdlwin, sdlglcontext);

    while (running) {
        while (SDL_PollEvent(&sdlevent)) {
            switch (sdlevent.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }

                case SDL_FINGERUP: {
                    break;
                }

                default: {
                    break;
                }
            }
        }

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(psdlwin);
        SDL_Delay(frameratethreadelay);
    }

    SDL_Quit();
    return 0;
}