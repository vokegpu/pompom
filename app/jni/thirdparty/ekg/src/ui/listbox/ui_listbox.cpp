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

#include "ekg/ui/listbox/ui_listbox.hpp"
#include "ekg/util/gui.hpp"

ekg::ui::listbox *ekg::ui::listbox::set_width(float w) {
    if (this->sync_ui.w != w) {
        this->sync_ui.w = w;

        ekg::bitwise::add(this->sync_flags, (uint16_t) ekg::ui_sync::dimension);
        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }

    return this;
}

float ekg::ui::listbox::get_width() {
    return this->rect_widget.w;
}

ekg::ui::listbox *ekg::ui::listbox::set_scaled_height(int32_t h) {
    if (this->scaled_height != h) {
        this->scaled_height = h;

        ekg::reload(this->id);
        ekg::synclayout(this->parent_id);
    }

    return this;
}

int32_t ekg::ui::listbox::get_scaled_height() {
    return this->scaled_height;
}

float ekg::ui::listbox::get_height() {
    return this->rect_widget.h;
}

ekg::ui::listbox *ekg::ui::listbox::set_place(uint16_t dock) {
    if (this->dock_flags != dock) {
        this->dock_flags = dock;
        ekg::synclayout(this->parent_id);
    }

    return this;
}

ekg::item &ekg::ui::listbox::item() {
    return this->data_item;
}

ekg::ui::listbox *ekg::ui::listbox::set_category_font_size(ekg::font font) {
    if (this->category_font_size != font) {
        this->category_font_size = font;
    }

    return this;
}

ekg::font ekg::ui::listbox::get_category_font_size() {
    return this->category_font_size;
}

ekg::ui::listbox *ekg::ui::listbox::set_item_font_size(ekg::font font) {
    if (this->item_font_size != font) {
        this->item_font_size = font;
    }

    return this;
}

ekg::font ekg::ui::listbox::get_item_font_size() {
    return this->item_font_size;
}