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

#ifndef EKG_UI_LISTBOX_H
#define EKG_UI_LISTBOX_H

#include "ekg/ui/abstract/ui_abstract.hpp"
#include "ekg/util/io.hpp"

namespace ekg::ui {
    class listbox : public ekg::ui::abstract {
    protected:
        ekg::item data_item {};
        ekg::font category_font_size {};
        ekg::font item_font_size {};
    public:
        ekg::ui::listbox *set_width(float w);
        float get_width();

        ekg::ui::listbox *set_scaled_height(int32_t h);
        int32_t get_scaled_height();
        float get_height();

        ekg::ui::listbox *set_place(uint16_t dock);
        ekg::item &item();

        ekg::ui::listbox *set_category_font_size(ekg::font font);
        ekg::font get_category_font_size();

        ekg::ui::listbox *set_item_font_size(ekg::font font);
        ekg::font get_item_font_size();
    };
}

#endif