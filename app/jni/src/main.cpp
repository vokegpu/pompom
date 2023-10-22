#include "ekg/ekg.hpp"
#include "catandroidassetmanager.hpp"

ekg::rect window_size {0, 0, 1280, 720};
bool running {true};


std::string checkcalc(std::string_view text, std::string_view operatortext) {
    std::string result {};
    result += text.substr(text.size() - 1, 1);
    if ((result == "*" || result == "-" || result == "+")) {
        result.clear();
        result += text.substr(0, text.size() - 1);
        result += operatortext;
    } else {
        result.clear();
        result += text;
        result += operatortext;
    }

    return result;
}

std::string resultcalc(std::string_view text) {
    std::string result {};

    int64_t lpom {};
    int64_t rpom {};
    int64_t cpom {};

    bool firstoperation {true};
    bool reset {};

    uint64_t find {};
    uint64_t size {text.size()};

    std::string textsubstr {text};
    std::string aliasoperator[3] {
            "*", "-", "+"
    };

    uint64_t it {};
    char prevoperator {};
    bool docalc {};
    bool kissme {};

    for (it = 0; it < size; it++) {
        if (!docalc) {
            switch (textsubstr.at(it)) {
                case '*':
                    cpom = std::stoi(result);
                    result.clear();
                    docalc = true;
                    break;
                case '-':
                    cpom = std::stoi(result);
                    result.clear();
                    docalc = true;
                    break;
                case '+':
                    cpom = std::stoi(result);
                    result.clear();
                    docalc = true;
                    break;
                default:
                    result += textsubstr.at(it);
                    break;
            }

            prevoperator = textsubstr.at(it);
            continue;
        }

        kissme = false;
        switch (textsubstr.at(it)) {
            case '*':
                kissme = true;
                break;
            case '-':
                kissme = true;
                break;
            case '+':
                kissme = true;
                break;
            default:
                kissme = it == size - 1;
                if (kissme) result += textsubstr.at(it);
                break;
        }

        if (!kissme) {
            result += textsubstr.at(it);
            continue;
        }

        switch (prevoperator) {
            case '*':
                cpom = cpom * std::stoi(result);
                result.clear();
                prevoperator = textsubstr.at(it);
                break;
            case '-':
                cpom = cpom - std::stoi(result);
                result.clear();
                prevoperator = textsubstr.at(it);
                break;
            case '+':
                cpom = cpom + std::stoi(result);
                result.clear();
                prevoperator = textsubstr.at(it);
                break;
            default:
                break;
        }
    }

    result = std::to_string(cpom);
    return result;
}

static uint64_t framerate {};

int32_t main(int32_t, char**) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_DisplayMode sdldisplaymode{};
    SDL_GetDisplayMode(0, 0, &sdldisplaymode);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

    SDL_Window *psdlwin{SDL_CreateWindow("pompom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         sdldisplaymode.w, sdldisplaymode.h, SDL_WINDOW_OPENGL)};
    SDL_GLContext sdlglcontext{SDL_GL_CreateContext(psdlwin)};
    SDL_Event sdl_event{};

    bool running{true};
    uint32_t frameratethreadelay{1000 / 60};

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

    ekg::autoscale = false;
    ekg::scalebase = {1280.0f, 768.0f};
    ekg::runtime ekg_runtime{};
    ekg::init(&ekg_runtime, psdlwin, cat::asset::abs("whitneybook.otf"));


    auto f1 = ekg::frame("sou gostosa", {700, 300}, {200, 200})->set_drag(
            ekg::dock::top)->set_resize(ekg::dock::bottom | ekg::dock::left | ekg::dock::right);

    ekg::button("swap locked-framerate")->set_callback(
            new ekg::task{"theme-switcher", nullptr, [](void *pdata) {
                framerate = framerate == 0 ? 16 : 0;
                SDL_GL_SetSwapInterval(framerate ? 1 : 0);
            }});

    ekg::label("oioioioioio", ekg::dock::fill);
    auto ff = ekg::frame("frame-debug", {200, 200}, ekg::dock::fill | ekg::dock::next);
    ff->set_resize(ekg::dock::right | ekg::dock::bottom);
    ff->set_height(300.0f);

    auto ff3 = ekg::textbox("frame-debug", "oi", ekg::dock::fill | ekg::dock::next);
    ff3->set_scaled_height(16);
    ekg::checkbox("Auto-scale", true, ekg::dock::fill | ekg::dock::next)->set_tag(
            "base.resolution.autoscale");
    ekg::label("Aspect:", ekg::dock::fill | ekg::dock::next);
    ekg::slider("base.resolution.width", 1920.0f, 800.0f, 2560.0f, ekg::dock::fill);
    ekg::slider("base.resolution.height", 1080.0f, 600.0f, 1440.0f, ekg::dock::fill);
    ff->add_child(ekg::scroll("hello-sou-gostosa")->get_id());
    f1->add_child(ff->get_id());
    f1->add_child(ekg::scroll("hello-sou-gostosa")->get_id());

    auto framedebug = ekg::frame("frame-debug", {0, 0}, {400, 400});
    framedebug->set_resize(ekg::dock::right | ekg::dock::bottom);
    framedebug->set_drag(ekg::dock::top);

    auto debuglabel = ekg::label("Debug:", ekg::dock::next);
    debuglabel->set_text_align(ekg::dock::left | ekg::dock::top);
    auto textboxdebug = ekg::textbox("textbox-debug", "", ekg::dock::next);
    textboxdebug->set_scaled_height(6);
    textboxdebug->set_enabled(false);
    textboxdebug->set_width(600.0f);

    ekg::ui::label *labelresult{};
    std::string previous_operator{};
    ekg::ui::frame *frame2{};

    for (int32_t it{}; it < 1; it++) {
        auto frame1 = ekg::frame("cat", {400, 400}, ekg::dock::none);
        frame1->set_resize(ekg::dock::right | ekg::dock::bottom | ekg::dock::left);

        ekg::checkbox("meow enabled", false, ekg::dock::next);
        ekg::checkbox("meow enabled", false, ekg::dock::next);
        ekg::checkbox("meow enabled", false);
        ekg::checkbox("meow enabled", false, ekg::dock::next);

        // @TODO remove text from UI slider

        auto slider = ekg::slider("gato", 0.0f, 0.0f, 1000000.0f, ekg::dock::fill);
        slider->set_text_align(ekg::dock::center);
        slider->set_precision(23);
        ekg::checkbox("meow enabled", false, ekg::dock::fill | ekg::dock::next);
        ekg::scroll("pompom-meow");

        framedebug->add_child(frame1->get_id());
        frame2 = ekg::frame("cha", {400, 400}, ekg::dock::none);
        frame2->set_resize(ekg::dock::right | ekg::dock::bottom | ekg::dock::left);
        framedebug->add_child(frame2->get_id());

        ekg::theme().gen_default_dark_theme();
        ekg::checkbox("Light Theme", false, ekg::dock::fill | ekg::dock::next)->set_callback(
                new ekg::task{"theme-switcher", frame2, [](void *pdata) {
                    ekg::ui::frame *frame{static_cast<ekg::ui::frame *>(pdata)};
                    // frame->set_pos(20, 20);
                    // @TODO fix random issue with set positon

                    auto &theme{ekg::theme()};
                    std::string current_theme_name{theme.get_current_theme_name()};

                    if (current_theme_name == "dark") {
                        theme.gen_default_light_theme();
                    } else if (current_theme_name == "light") {
                        theme.gen_default_dark_theme();
                    }
                }});

        labelresult = ekg::label("0", ekg::dock::fill | ekg::dock::next);
        labelresult->set_scaled_height(4);
        labelresult->set_text_align(ekg::dock::right | ekg::dock::bottom);
        labelresult->set_font_size(ekg::font::big);
        labelresult->set_tag("calculator-result");

        auto buselesstop1 = ekg::button("", ekg::dock::fill | ekg::dock::next);
        buselesstop1->set_scaled_height(2);
        buselesstop1->set_text_align(ekg::dock::center);

        auto buselesstop2 = ekg::button("", ekg::dock::fill);
        buselesstop2->set_scaled_height(2);
        buselesstop2->set_text_align(ekg::dock::center);

        auto bcls = ekg::button("cls", ekg::dock::fill);
        bcls->set_scaled_height(2);
        bcls->set_text_align(ekg::dock::center);
        bcls->set_tag("calculator-cls");

        auto berase = ekg::button("<=", ekg::dock::fill);
        berase->set_scaled_height(2);
        berase->set_text_align(ekg::dock::center);
        berase->set_tag("calculator-erase");

        auto b7 = ekg::button("7", ekg::dock::fill | ekg::dock::next);
        b7->set_scaled_height(2);
        b7->set_text_align(ekg::dock::center);

        auto b8 = ekg::button("8", ekg::dock::fill);
        b8->set_scaled_height(2);
        b8->set_text_align(ekg::dock::center);

        auto b9 = ekg::button("9", ekg::dock::fill);
        b9->set_scaled_height(2);
        b9->set_text_align(ekg::dock::center);

        auto bmultiply = ekg::button("x", ekg::dock::fill);
        bmultiply->set_scaled_height(2);
        bmultiply->set_text_align(ekg::dock::center);
        bmultiply->set_tag("calculator-multiply");

        auto b4 = ekg::button("4", ekg::dock::fill | ekg::dock::next);
        b4->set_scaled_height(2);
        b4->set_text_align(ekg::dock::center);

        auto b5 = ekg::button("5", ekg::dock::fill);
        b5->set_scaled_height(2);
        b5->set_text_align(ekg::dock::center);

        auto b6 = ekg::button("6", ekg::dock::fill);
        b6->set_scaled_height(2);
        b6->set_text_align(ekg::dock::center);

        auto bsubtract = ekg::button("-", ekg::dock::fill);
        bsubtract->set_scaled_height(2);
        bsubtract->set_text_align(ekg::dock::center);
        bsubtract->set_tag("calculator-subtract");

        auto b1 = ekg::button("1", ekg::dock::fill | ekg::dock::next);
        b1->set_scaled_height(2);
        b1->set_text_align(ekg::dock::center);

        auto b2 = ekg::button("2", ekg::dock::fill);
        b2->set_scaled_height(2);
        b2->set_text_align(ekg::dock::center);

        auto b3 = ekg::button("3", ekg::dock::fill);
        b3->set_scaled_height(2);
        b3->set_text_align(ekg::dock::center);

        auto baddition = ekg::button("+", ekg::dock::fill);
        baddition->set_scaled_height(2);
        baddition->set_text_align(ekg::dock::center);
        baddition->set_tag("calculator-addition");

        auto buseless1 = ekg::button("", ekg::dock::fill | ekg::dock::next);
        buseless1->set_scaled_height(2);
        buseless1->set_text_align(ekg::dock::center);

        auto b0 = ekg::button("0", ekg::dock::fill);
        b0->set_scaled_height(2);
        b0->set_text_align(ekg::dock::center);

        auto buseless2 = ekg::button("", ekg::dock::fill);
        buseless2->set_scaled_height(2);
        buseless2->set_text_align(ekg::dock::center);

        auto bassign = ekg::button("=", ekg::dock::fill);
        bassign->set_scaled_height(2);
        bassign->set_text_align(ekg::dock::center);
        bassign->set_tag("calculator-assign");
        ekg::scroll("pompom-calc");
        ekg::pop_group();

        framedebug->add_child(ekg::frame("helko", {300, 300}, ekg::dock::next)->get_id());
        ekg::button("hello", ekg::dock::fill);
        ekg::button("hello vc é lindo", ekg::dock::next);
        ekg::pop_group();
    }

    framedebug->add_child(ekg::scroll("mewoscroll")->get_id());

    // ekg::button("Button 7", ekg::dock::right);
    // ekg::button("Button 8", ekg::dock::fill | ekg::dock::bottom);

    uint64_t cpu_now_ticks{};
    uint64_t cpu_last_ticks{};
    ekg::event event{};

    ekg::input::bind("pompom", "c-up");
    ekg::input::bind("pompom", "rshift");

    glDisable(GL_DEPTH_TEST);

    ekg::timing mainloop_timing {};
    ekg::timing fps_timing {};

    uint64_t fps {60};
    uint64_t fps_ms_interval {1000 / fps};
    uint64_t display_fps {};
    uint64_t ticked_frames {};


    /*
     * Mainloop.
     */
    while (running) {
        cpu_last_ticks = cpu_now_ticks;
        cpu_now_ticks = SDL_GetPerformanceCounter();
        ekg::display::dt = static_cast<float>(cpu_now_ticks - cpu_last_ticks) /
                           static_cast<float>(SDL_GetPerformanceFrequency());

        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
                case SDL_QUIT: {
                    running = false;
                    break;
                }

                default: {
                    //framedebug->set_height(ekg::display::height);

                    if (ekg::listen(event, sdl_event)) {
                        if (event.tag == "file") {
                            if (event.value == "Copy") {
                                ekg::input::clipboard(ekg::clipboard::copy);
                            } else if (event.value == "Cut") {
                                ekg::input::clipboard(ekg::clipboard::cut);
                            } else if (event.value == "Paste") {
                                ekg::input::clipboard(ekg::clipboard::paste);
                            }
                        } else if (event.tag == "base.resolution.width") {
                            ekg::scalebase.x = std::stof(event.value);
                        } else if (event.tag == "base.resolution.height") {
                            ekg::scalebase.y = std::stof(event.value);
                        } else if (event.tag == "base.resolution.autoscale") {
                            ekg::autoscale = event.value == "checked";
                        }

                        if (event.tag == "calculator-assign") {
                            labelresult->set_text(resultcalc(labelresult->get_text()));
                        } else if (event.tag == "calculator-cls") {
                            labelresult->set_text("0");
                        } else if (event.tag == "calculator-addition") {
                            labelresult->set_text(checkcalc(labelresult->get_text(), "+"));
                        } else if (event.tag == "calculator-subtract") {
                            labelresult->set_text(checkcalc(labelresult->get_text(), "-"));
                        } else if (event.tag == "calculator-multiply") {
                            labelresult->set_text(checkcalc(labelresult->get_text(), "*"));
                        } else if (event.tag == "calculator-erase") {
                            std::string text{labelresult->get_text()};
                            if (text.size() <= 1) {
                                text = "0";
                            } else {
                                text = text.substr(0, text.size() - 1);
                            }

                            labelresult->set_text(text);
                        } else {
                            std::string number{};
                            for (uint32_t i{}; i < 10; i++) {
                                number = std::to_string(i);
                                if (event.tag == number) {
                                    labelresult->set_text(labelresult->get_text() == "0" ? number :
                                                          labelresult->get_text() + number);
                                    break;
                                }
                            }
                        }
                    }

                    ekg::poll_event(sdl_event);

                    if (ekg::input::released() && ekg::input::receive("mouse-3-up")) {
                        auto main = ekg::popup("file", {"---Add", "Cut", "Copy", "---Paste",
                                                        "---Select All", "Actions"});
                        auto three = ekg::popup("file-add",
                                                {"Cube", "Plane", "Sphere", "Hexagon", "Hexagon"});
                        auto hexagon = ekg::popup("file-add-hexagon",
                                                  {"Tree D", "Plane", "Double Pairs Daggers"});
                        auto game = ekg::popup("file-actions", {"Reload Clock", "Flush"});

                        if (main) {
                            main->link("Add", three);
                            main->link("Actions", game);
                            three->link("Hexagon", hexagon);
                        }
                    }

                    if (ekg::input::action("pompom")) {
                        ekg::log() << "VASCO GAMMA E JOAO É GOSTOSAA";
                    }

                    break;
                }
            }
        }

        // framedebug->set_size(ekg::display::width, ekg::display::height);

        if (ekg::log::buffered) {
            std::string old_log{textboxdebug->get_text()};
            if (old_log.size() > 50000) old_log = '\n';
            old_log += ekg::log::buffer.str();
            textboxdebug->set_text(old_log);
        }

        ekg::update();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        ekg::render();

        // Count the FPS.
        ticked_frames++;

        if (ekg::reach(fps_timing, 1000) && ekg::reset(fps_timing)) {
            display_fps = ticked_frames;
            debuglabel->set_text("FPS: " + std::to_string(display_fps));
            ticked_frames = 0;
        }

        SDL_GL_SwapWindow(psdlwin);

        // Swap buffers.
        if (framerate) {
            SDL_Delay(framerate);
        }
    }
}

