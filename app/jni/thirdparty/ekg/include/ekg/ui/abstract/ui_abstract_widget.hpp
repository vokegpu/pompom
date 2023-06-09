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

#ifndef EKG_UI_ABSTRACT_WIDGET_H
#define EKG_UI_ABSTRACT_WIDGET_H

#include "ekg/ui/abstract/ui_abstract.hpp"
#include "ekg/os/ekg_sdl.hpp"

namespace ekg::ui {
    class abstract_widget : public ekg::feature {
    public:
        ekg::ui::abstract *data {};
        ekg::flag flag {};
        ekg::rect dimension {};
        ekg::vec2 min_size {};

        ekg::rect *parent {};
        ekg::vec4 *scroll {};
        
        ekg::rect empty_parent {};
        ekg::vec4 empty_scroll {};

        bool is_scissor_refresh {};
        bool is_high_frequency {};
        bool is_targeting_absolute_parent {};
    public:
        ekg::rect get_static_rect();
        [[nodiscard]] ekg::rect &get_abs_rect();
    public:
        explicit abstract_widget();
        ~abstract_widget();

        virtual void on_destroy();
        virtual void on_reload();
        virtual void on_pre_event(SDL_Event &sdl_event);
        virtual void on_event(SDL_Event &sdl_event);
        virtual void on_post_event(SDL_Event &sdl_event);
        virtual void on_update();
        virtual void on_draw_refresh();
    };
}

#endif