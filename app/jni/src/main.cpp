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

    cat::init();
    if (cat::asset::extract("pompom/", "Whitney-Black.ttf")) {
        ekg::log() << "Failed to extract pompom file ttf";
    } else {
        ekg::log() << "!!! extracteddd pompom file ttf";
    }

    ekg::gl_version = "#version 300 es \nprecision highp float;";
    ekg::init(psdlwin, cat::asset::abs("Whitney-Black.ttf"));

    auto frame2 = ekg::frame("pompom", {20, 600}, {500, 500});
    frame2->set_resize(ekg::dock::left | ekg::dock::bottom | ekg::dock::right);
    frame2->set_drag(ekg::dock::top);

    ekg::label("EKG running on Android NDK", ekg::dock::fill);
    ekg::button("Press here if you love cats", ekg::dock::fill | ekg::dock::next);
    ekg::button("Pom Pom", ekg::dock::next);
    ekg::slider("Sliderrr", -200.0f, -300.0f, 300.0f);
    ekg::textbox("Pompom", "gi", ekg::dock::fill | ekg::dock::next)->set_scaled_height(32);

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
