
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

#ifndef EKG_CORE_H
#define EKG_CORE_H

#include "ekg/ui/abstract/ui_abstract_widget.hpp"
#include "ekg/service/handler.hpp"
#include "ekg/draw/font.hpp"
#include "ekg/service/theme.hpp"
#include "ekg/service/input.hpp"
#include "ekg/service/layout.hpp"
#include "ekg/util/io.hpp"
#include "ekg/util/gui.hpp"
#include <unordered_map>

namespace ekg {
    /* The swap maps for prevent copies of hashes and vector. */
    struct swap {
        static ekg::stack collect;
        static ekg::stack back;
        static ekg::stack front;
        static std::vector<ekg::ui::abstract_widget*> buffer;
        static std::vector<uint64_t> tooktimeanalyzingtelemtry;

        static void refresh();
    };

    /* The main runtime for run ekg. */
    class runtime {
    protected:
        void prepare_tasks();
        void prepare_ui_env();
        void erase(int32_t id);
    private:
        /* Widget env lists and maps for tasks. */

        std::unordered_map<std::string, std::vector<ekg::ui::abstract_widget*>> widget_list_map {};
        std::unordered_map<int32_t, ekg::ui::abstract_widget*> widget_map {};
        std::unordered_map<int32_t, bool> processed_widget_map {};

        /* Core services and instances. */

        ekg::ui::abstract *current_bind_group {};
        ekg::ui::abstract_widget *widget_absolute_activy {};

        int32_t token_id {};
        int32_t widget_id_focused {};
        int32_t prev_widget_id_focused {};
        int32_t swap_widget_id_focused {};
        int32_t widget_id_pressed_focused {};
        int32_t widget_id_released_focused {};

        bool enable_high_priority_frequency {};
        bool should_re_batch_gui {};
    public:
        ekg::service::input service_input {};
        ekg::service::theme service_theme {};
        ekg::service::handler service_handler {};
        ekg::service::layout service_layout {};

        ekg::draw::font_renderer f_renderer_small {};
        ekg::draw::font_renderer f_renderer_normal {};
        ekg::draw::font_renderer f_renderer_big {};

        ekg::gpu::allocator gpu_allocator {};
        ekg::timing ui_timing {};
        SDL_Window* root {};
    public:
        ekg::ui::abstract_widget* get_fast_widget_by_id(int32_t id);

        void gen_widget(ekg::ui::abstract *ui);
        void do_task_reload(ekg::ui::abstract_widget *widget);
        void do_task_synclayout(ekg::ui::abstract_widget *widget);
        void do_task_refresh(ekg::ui::abstract_widget *widget);
        void set_update_high_frequency(ekg::ui::abstract_widget *widget);
        void request_redraw_gui();

        void end_group_flag();
        void update_size_changed();

        void init();
        void quit();

        void process_event(SDL_Event &sdl_event);
        void process_update();
        void process_render();
    };
};

#endif