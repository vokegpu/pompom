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

    cat::asset::extract("pompom/", "whitneybook.otf");
    ekg::glsl_version = "#version 300 es \nprecision highp float;";

    ekg::runtime ekg_runtime {};
    ekg::init(&ekg_runtime, psdlwin, cat::asset::abs("whitneybook.otf"));

    auto framedebug = ekg::frame("frame-debug", {0, 0}, {static_cast<float>(sdldisplaymode.w), static_cast<float>(sdldisplaymode.h)});
    framedebug->set_resize(ekg::dock::right | ekg::dock::bottom);

    ekg::ui::label *labelresult {};
    std::string previous_operator {};
    ekg::ui::frame *frame2 {};

    int32_t scaled {64};
    int32_t button_scaled {4};

    ekg::log() << "resolution: " << sdldisplaymode.w << ", " << sdldisplaymode.h;

    for (int32_t it {}; it < 1; it++) {
        ekg::theme().gen_default_dark_theme();
        ekg::checkbox("Light Theme", false, ekg::dock::fill | ekg::dock::next)->set_callback(new ekg::task {"theme-switcher", frame2, [](void *pdata) {
            ekg::ui::frame *frame {static_cast<ekg::ui::frame*>(pdata)};

            auto &theme {ekg::theme()};
            std::string current_theme_name {theme.get_current_theme_name()};

            if (current_theme_name == "dark") {
                theme.gen_default_light_theme();
            } else if (current_theme_name == "light") {
                theme.gen_default_dark_theme();
            }
        }});

        labelresult = ekg::label("0", ekg::dock::fill | ekg::dock::next);
        labelresult->set_scaled_height(scaled);
        labelresult->set_text_align(ekg::dock::right | ekg::dock::bottom);
        labelresult->set_font_size(ekg::font::big);
        labelresult->set_tag("calculator-result");

        auto buselesstop1 = ekg::button("", ekg::dock::fill | ekg::dock::next);
        buselesstop1->set_scaled_height(button_scaled);
        buselesstop1->set_text_align(ekg::dock::center);

        auto buselesstop2 = ekg::button("", ekg::dock::fill);
        buselesstop2->set_scaled_height(button_scaled);
        buselesstop2->set_text_align(ekg::dock::center);

        auto bcls = ekg::button("cls", ekg::dock::fill);
        bcls->set_scaled_height(button_scaled);
        bcls->set_text_align(ekg::dock::center);
        bcls->set_tag("calculator-cls");

        auto berase = ekg::button("<=", ekg::dock::fill);
        berase->set_scaled_height(button_scaled);
        berase->set_text_align(ekg::dock::center);
        berase->set_tag("calculator-erase");

        auto b7 = ekg::button("7", ekg::dock::fill | ekg::dock::next);
        b7->set_scaled_height(button_scaled);
        b7->set_text_align(ekg::dock::center);

        auto b8 = ekg::button("8", ekg::dock::fill);
        b8->set_scaled_height(button_scaled);
        b8->set_text_align(ekg::dock::center);

        auto b9 = ekg::button("9", ekg::dock::fill);
        b9->set_scaled_height(button_scaled);
        b9->set_text_align(ekg::dock::center);

        auto bmultiply = ekg::button("x", ekg::dock::fill);
        bmultiply->set_scaled_height(button_scaled);
        bmultiply->set_text_align(ekg::dock::center);
        bmultiply->set_tag("calculator-multiply");

        auto b4 = ekg::button("4", ekg::dock::fill | ekg::dock::next);
        b4->set_scaled_height(button_scaled);
        b4->set_text_align(ekg::dock::center);

        auto b5 = ekg::button("5", ekg::dock::fill);
        b5->set_scaled_height(button_scaled);
        b5->set_text_align(ekg::dock::center);

        auto b6 = ekg::button("6", ekg::dock::fill);
        b6->set_scaled_height(button_scaled);
        b6->set_text_align(ekg::dock::center);

        auto bsubtract = ekg::button("-", ekg::dock::fill);
        bsubtract->set_scaled_height(button_scaled);
        bsubtract->set_text_align(ekg::dock::center);
        bsubtract->set_tag("calculator-subtract");

        auto b1 = ekg::button("1", ekg::dock::fill | ekg::dock::next);
        b1->set_scaled_height(button_scaled);
        b1->set_text_align(ekg::dock::center);

        auto b2 = ekg::button("2", ekg::dock::fill);
        b2->set_scaled_height(button_scaled);
        b2->set_text_align(ekg::dock::center);

        auto b3 = ekg::button("3", ekg::dock::fill);
        b3->set_scaled_height(button_scaled);
        b3->set_text_align(ekg::dock::center);

        auto baddition = ekg::button("+", ekg::dock::fill);
        baddition->set_scaled_height(button_scaled);
        baddition->set_text_align(ekg::dock::center);
        baddition->set_tag("calculator-addition");

        auto buseless1 = ekg::button("", ekg::dock::fill | ekg::dock::next);
        buseless1->set_scaled_height(button_scaled);
        buseless1->set_text_align(ekg::dock::center);

        auto b0 = ekg::button("0", ekg::dock::fill);
        b0->set_scaled_height(button_scaled);
        b0->set_text_align(ekg::dock::center);

        auto buseless2 = ekg::button("", ekg::dock::fill);
        buseless2->set_scaled_height(button_scaled);
        buseless2->set_text_align(ekg::dock::center);

        auto bassign = ekg::button("=", ekg::dock::fill);
        bassign->set_scaled_height(button_scaled);
        bassign->set_text_align(ekg::dock::center);
        bassign->set_tag("calculator-assign");
        ekg::pop_group();
    }

    framedebug->add_child(ekg::scroll("mewoscroll")->get_id());

    while (running) {
        while (SDL_PollEvent(&sdlevent)) {
            switch (sdlevent.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }

                default: {
                    ekg::poll_event(sdlevent);
                    break;
                }
            }
        }

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ekg::display::dt = 0.016f;
        ekg::update();
        ekg::render();

        SDL_GL_SwapWindow(psdlwin);
        SDL_Delay(frameratethreadelay);
    }

    SDL_Quit();
    return 0;
}
