/*
 * MIT License
 * 
 * Copyright (c) 2022-2023 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ekg/ui/checkbox/ui_checkbox.hpp"
#include "ekg/util/gui.hpp"

ekg::ui::checkbox *ekg::ui::checkbox::set_callback(task *event) {
    if (this->callback != event && this->callback != nullptr) {
        delete this->callback;
        this->callback = nullptr;
    }

    this->callback = event;
    return this;
}

ekg::task *ekg::ui::checkbox::get_callback() {
    return this->callback;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_font_size(ekg::font font) {
    if (this->font_size != font) {
        this->font_size = font;

        ekg::synclayout(this->parent_id);
        ekg::reload(this->id);
    }

    return this;
}

ekg::font ekg::ui::checkbox::get_font_size() {
    return this->font_size;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_place(uint16_t docks) {
    if (this->dock_flags != docks) {
        this->dock_flags = docks;

        ekg::synclayout(this->id);
    }

    return this;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_width(float w) {
    if (this->sync_ui.w != w) {
        this->sync_ui.w = w;
        ekg::bitwise::add(this->sync_flags, (uint16_t) ekg::ui_sync::dimension);
        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }

    return this;
}

float ekg::ui::checkbox::get_width() {
    return this->rect_widget.w;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_scaled_height(int32_t scaled_height_factor) {
    if (this->scaled_height != scaled_height_factor) {
        this->scaled_height = scaled_height_factor;
        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }

    return this;
}

int32_t ekg::ui::checkbox::get_scaled_height() {
    return this->scaled_height;
}

float ekg::ui::checkbox::get_height() {
    return this->rect_widget.h;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_text(std::string_view string) {
    if (this->text != string) {
        this->text = string;

        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }

    return this;
}

std::string_view ekg::ui::checkbox::get_text() {
    return this->text;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_value(bool val) {
    if (this->value != val) {
        this->value = val;
        ekg::dispatch(ekg::env::redraw);
    }

    return this;
}

bool ekg::ui::checkbox::get_value() {
    return this->value;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_text_align(uint16_t docks) {
    if (this->dock_text != docks) {
        this->dock_text = docks;
        ekg::reload(this->id);
    }

    return this;
}

uint16_t ekg::ui::checkbox::get_text_align() {
    return this->dock_text;
}

ekg::ui::checkbox *ekg::ui::checkbox::set_box_align(uint16_t docks) {
    if (this->dock_box != docks) {
        this->dock_box = docks;
        ekg::reload(this->id);
    }

    return this;
}

uint16_t ekg::ui::checkbox::get_box_align() {
    return this->dock_box;    
}