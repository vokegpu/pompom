/*
 * VOKEGPU EKG LICENSE
 *
 * Respect ekg license policy terms, please take a time and read it.
 * 1- Any "skidd" or "stole" is not allowed.
 * 2- Forks and pull requests should follow the license policy terms.
 * 3- For commercial use, do not sell without give credit to vokegpu ekg.
 * 4- For ekg users and users-programmer, we do not care, free to use in anything (utility, hacking, cheat, game, software).
 * 5- Malware, rat and others virus. We do not care.
 * 6- Do not modify this license under any instance.
 *
 * @VokeGpu 2023 all rights reserved.
 */

#include "ekg/service/theme.hpp"
#include "ekg/util/env.hpp"
#include <filesystem>

void ekg::service::theme::init() {
    ekg::log() << "Initialising default theme";

    this->gen_default_theme();
    this->refresh_theme_list();
}

void ekg::service::theme::quit() {

}

void ekg::service::theme::gen_default_theme() {
    this->frame_background = {70, 81, 109, 255};
    this->frame_border = {190, 190, 190, 0};
    this->frame_outline = {30, 40, 60, 100};
    this->frame_activy_offset = 18;

    this->button_string = {202, 207, 222, 255};
    this->button_background = {53, 61, 81, 255};
    this->button_activy = {44, 166, 255, 200};
    this->button_outline = {202, 207, 222, 150};
    this->button_highlight = {44, 166, 255, 50};

    this->checkbox_string = {202, 207, 222, 255};
    this->checkbox_background = {53, 61, 81, 255};
    this->checkbox_activy = {44, 166, 255, 200};
    this->checkbox_outline = {202, 207, 222, 150};
    this->checkbox_highlight = {44, 166, 255, 50};

    this->slider_string = {202, 207, 222, 255};
    this->slider_background = {53, 61, 81, 255};
    this->slider_activy = {44, 166, 255, 200};
    this->slider_outline = {202, 207, 222, 150};
    this->slider_highlight = {44, 166, 255, 50};
    this->slider_activy_bar = this->slider_activy;
    this->slider_bar_thickness = 16;
    this->slider_target_thickness = 32;

    this->label_string = {202, 207, 222, 255};

    this->popup_string = {202, 207, 222, 255};
    this->popup_background = {53, 61, 81, 255};
    this->popup_outline = {30, 40, 60, 100};
    this->popup_highlight = {44, 166, 255, 50};
    this->popup_drop_animation_delay = 120;

    this->textbox_string = {2, 7, 22, 255};
    this->textbox_background = {202, 207, 222, 255};
    this->textbox_outline = {202, 207, 222, 150};
    this->textbox_select = {2, 7, 222, 255};
    this->textbox_cursor = {2, 7, 22, 255};

    this->scrollbar_background = {53, 61, 81, 255};
    this->scrollbar_outline = {202, 207, 222, 150};
    this->scrollbar_activy = {44, 166, 255, 200};
    this->scrollbar_highlight = {44, 166, 255, 50};
    this->scrollbar_pixel_thickness = 5;
}

void ekg::service::theme::refresh_theme_list() {
    ekg::log() << "Analysing files for themes";

    this->loaded_theme_list.clear();
    std::string file_extension {};

    for (auto const &files : std::filesystem::directory_iterator {{"./"}}) {
        file_extension = files.path().filename().string();

        if (file_extension.length() < 4) {
            continue;
        }

        file_extension = file_extension.substr(file_extension.size() - 4, file_extension.size());

        if (file_extension != ".ekg") {
            continue;
        }

        file_extension = files.path().filename().string();
        this->loaded_theme_list.push_back(file_extension);

        ekg::log() << "Found theme: " << file_extension;
    }
}

void ekg::service::theme::load_theme(const std::string &theme) {
    bool keep_load_theme {};

    for (std::string &themes : this->loaded_theme_list) {
        keep_load_theme = themes == theme;

        if (keep_load_theme) {
            break;
        }
    }

    if (keep_load_theme) {
        // todo add theme load.
    }
}
