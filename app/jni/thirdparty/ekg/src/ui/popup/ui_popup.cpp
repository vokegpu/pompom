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

#include "ekg/ui/popup/ui_popup.hpp"

int64_t ekg::ui::popup::contains(std::string_view component_name) {
    for (uint32_t it {}; it < this->component_list.size(); it++) {
        if (this->component_list.at(it).name == component_name) {
            return it;
        }
    }

    return -1;
}

void ekg::ui::popup::append(const std::vector<std::string> &component_name_list) {
    for (const std::string &component_name : component_name_list) {
        this->append(component_name);
    }
}

void ekg::ui::popup::append(std::string_view component_name) {
    std::string factored_component_name {component_name};
    bool is_separator {component_name.size() >= 3 && component_name.at(0) == '-' && component_name.at(1) == '-' && component_name.at(2) == '-'};

    if (is_separator) {
        factored_component_name = factored_component_name.substr(3, factored_component_name.size());
        is_separator = true;
    }

    if (this->contains(factored_component_name) != -1) {
        return;
    }

    ekg::component component {};
    component.name = factored_component_name;
    component.boolean = is_separator;
    this->component_list.push_back(component);
}

void ekg::ui::popup::append_linked(std::string_view component_name, ekg::ui::popup *popup_linked) {
    int64_t index {this->contains(component_name)};

    if (index == -1) {
        this->append(component_name);
    }

    auto &component {this->component_list.at(index)};
    component.linked_id = 0;
    if (popup_linked != nullptr) {
        popup_linked->set_state(ekg::state::invisible);
        component.linked_id = popup_linked->get_id();
        this->add_child(component.linked_id);
    }
}

void ekg::ui::popup::remove(std::string_view component_name) {
    std::vector<ekg::component> new_list {};
    for (ekg::component &component : this->component_list) {
        if (component.name == component_name) {
            this->remove_child(component.linked_id);
            continue;
        }

        new_list.push_back(component);
    }

    this->component_list.clear();
    this->component_list.insert(this->component_list.end(), new_list.begin(), new_list.end());
}

std::vector<ekg::component> &ekg::ui::popup::get_component_list() {
    return this->component_list;
}

void ekg::ui::popup::set_text_align(uint16_t flags) {
    if (this->dock_flags != flags) {
        this->dock_flags = flags;
        ekg::reload(this->id);
    }
}

uint16_t ekg::ui::popup::get_text_align() {
    return this->dock_flags;
}

void ekg::ui::popup::set_width(float width) {
    if (this->sync_ui.w != width) {
        this->sync_ui.w = width;

        ekg::bitwise::add(this->sync_flags, (uint16_t) ekg::uisync::dimension);
        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }
}

float ekg::ui::popup::get_width() {
    return this->rect_widget.w;
}

void ekg::ui::popup::set_scaled_height(int32_t scaled_height_factor) {
    if (this->scaled_height != scaled_height_factor) {
        this->scaled_height = scaled_height_factor;

        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }
}

int32_t ekg::ui::popup::get_scaled_height() {
    return this->scaled_height;
}

float ekg::ui::popup::get_height() {
    return this->rect_widget.h;
}

void ekg::ui::popup::set_font_size(ekg::font f_size) {
    if (this->font_size != f_size) {
        this->font_size = f_size;

        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }
}

ekg::font ekg::ui::popup::get_font_size() {
    return this->font_size;
}

void ekg::ui::popup::set_pos(float x, float y) {
    if (this->sync_ui.x != x || this->sync_ui.y != y) {
        this->sync_ui.x = x;
        this->sync_ui.y = y;

        ekg::bitwise::add(this->sync_flags, (uint16_t) ekg::uisync::dimension);
        ekg::reload(this->id);
    }
}

ekg::vec2 ekg::ui::popup::get_pos() {
    return {this->rect_widget.x, this->rect_widget.y};
}