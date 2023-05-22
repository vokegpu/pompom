#include "ekg/ekg.hpp"
#include "catandroidassetmanager.hpp"

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

    std::string androidinternalpath {SDL_AndroidGetInternalStoragePath()};
    ekg::gl_version = "#version 300 es \nprecision highp float;";
    ekg::init(psdlwin, androidinternalpath + "/Whitney-Black.ttf");
    ekg::log() << androidinternalpath;

    auto frame = ekg::frame("Hello", {20, 20}, {500, 500});
    frame->set_resize(ekg::dock::left | ekg::dock::bottom | ekg::dock::right);
    frame->set_drag(ekg::dock::top);

    while (running) {
        while (SDL_PollEvent(&sdlevent)) {
            switch (sdlevent.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }

                default: {
                    ekg::event(sdlevent);
                    break;
                }
            }
        }

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ekg::update();
        ekg::render();

        SDL_GL_SwapWindow(psdlwin);
        SDL_Delay(frameratethreadelay);
    }

    SDL_Quit();
    return 0;
}
