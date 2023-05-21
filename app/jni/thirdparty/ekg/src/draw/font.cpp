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

#include "ekg/draw/font.hpp"
#include "ekg/util/env.hpp"
#include "ekg/os/info.hpp"

FT_Library ekg::draw::font_renderer::ft_library {};

float ekg::draw::font_renderer::get_text_width(std::string_view text) {
    if (text.empty()) {
        return 0;
    }

    FT_Vector ft_vec {};
    this->ft_uint_previous = 0;

    float text_width {};
    char32_t ui32char {};
    uint8_t ui8char {};
    std::string utf8string {};

    for (size_t it {}; it < text.size(); it++) {
        ui8char = static_cast<uint8_t>(text.at(it));
        it += ekg::utf8checksequence(ui8char, ui32char, utf8string, text, it);

        if (this->ft_bool_kerning && this->ft_uint_previous) {
            FT_Get_Kerning(this->ft_face, this->ft_uint_previous, ui32char, 0, &ft_vec);
            text_width += static_cast<float>(ft_vec.x >> 6);
        }

        this->ft_uint_previous = ui32char;
        text_width += this->allocated_char_data[ui32char].wsize;
    }

    return text_width;
}

float ekg::draw::font_renderer::get_text_height() {
    return this->text_height;
}

void ekg::draw::font_renderer::set_font(const std::string &path) {
    if (this->font_path != path) {
        this->font_path = path;
        this->reload();
    }
}

void ekg::draw::font_renderer::set_size(uint8_t size) {
    if (this->font_size != size) {
        this->font_size = size;
        this->reload();
    }
}

void ekg::draw::font_renderer::reload() {
    if (!this->flag_first_time) {
        FT_Done_Face(this->ft_face);
    }

    this->flag_unloaded = FT_New_Face(ekg::draw::font_renderer::ft_library, this->font_path.c_str(), 0, &this->ft_face);
    this->flag_first_time = false;

    if (this->flag_unloaded) {
        ekg::log(ekg::log::ERROR) << "Failed to create font face from '" << this->font_path << "'";
        return;
    }

    FT_Set_Pixel_Sizes(this->ft_face, 0, this->font_size);

    /* Phase of getting bitmap texture bounds. */
    this->full_width = 0;
    this->full_height = 0;

    this->ft_bool_kerning = FT_HAS_KERNING(this->ft_face);
    this->ft_glyph_slot = this->ft_face->glyph;

    for (char32_t char_codes {}; char_codes < 256; char_codes++) {
        if (FT_Load_Char(this->ft_face, char_codes, FT_LOAD_RENDER)) {
            continue;
        }

        this->full_width += static_cast<float>(this->ft_glyph_slot->bitmap.width);
        this->full_height = std::max(this->full_height, static_cast<float>(this->ft_glyph_slot->bitmap.rows));
    }
	
    this->text_height = this->full_height;
    this->offset_text_height = (this->text_height / 6) / 2;
    this->text_height += this->offset_text_height;

    /* Phase of getting chars metric_list. */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if (this->texture != 0) {
        glDeleteTextures(1, &this->texture);
    }

    auto internal_format {GL_RED};
    if (ekg::os == ekg::platform::os_android) {
        internal_format = GL_RGBA;
    }

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, (GLint) this->full_width, (GLint) this->full_height, 0, internal_format, GL_UNSIGNED_BYTE, nullptr);

    float offset {};
    for (char32_t char_codes {}; char_codes < 256; char_codes++) {
        if (FT_Load_Char(this->ft_face, char_codes, FT_LOAD_RENDER)) {
            continue;
        }

        ekg::char_data &char_data {this->allocated_char_data[char_codes]};
        char_data.x = offset / static_cast<float>(this->full_width);
        char_data.w = static_cast<float>(this->ft_glyph_slot->bitmap.width);
        char_data.h = static_cast<float>(this->ft_glyph_slot->bitmap.rows);

        char_data.left = static_cast<float>(this->ft_glyph_slot->bitmap_left);
        char_data.top = static_cast<float>(this->ft_glyph_slot->bitmap_top);
        char_data.wsize = static_cast<float>(this->ft_glyph_slot->advance.x >> 6);

        glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(offset), 0, static_cast<GLsizei>(char_data.w), static_cast<GLsizei>(char_data.h), GL_RED, GL_UNSIGNED_BYTE, this->ft_glyph_slot->bitmap.buffer);
        offset += char_data.w;
    }

#if defined(__ANDROID__)
#else
    GLint swizzle_format[] {GL_ZERO, GL_ZERO, GL_ZERO, GL_RED};

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle_format);
#endif
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ekg::draw::font_renderer::bind_allocator(ekg::gpu::allocator *gpu_allocator) {
    this->allocator = gpu_allocator;
}

void ekg::draw::font_renderer::blit(std::string_view text, float x, float y, const ekg::vec4 &color) {
    if (this->allocator == nullptr || this->flag_unloaded || text.empty()) {
        return;
    }

    x = static_cast<float>(static_cast<int32_t>(x));
    y = static_cast<float>(static_cast<int32_t>(y - this->offset_text_height));

    ekg::gpu::data &data {this->allocator->bind_current_data()};

    data.shape_rect[0] = x;
    data.shape_rect[1] = y;
    data.shape_rect[2] = static_cast<float>(ekg::concave);
    data.shape_rect[3] = static_cast<float>(ekg::concave);

    data.material_color[0] = static_cast<uint8_t>(color.x);
    data.material_color[1] = static_cast<uint8_t>(color.y);
    data.material_color[2] = static_cast<uint8_t>(color.z);
    data.material_color[3] = static_cast<uint8_t>(color.w);

    ekg::rect vertices {};
    ekg::rect coordinates {};
    ekg::char_data char_data {};

    x = 0;
    y = 0;

    data.factor = 1;

    char32_t ui32char {};
    uint8_t ui8char {};
    std::string utf8string {};
    size_t stringsize {};

    for (size_t it {}; it < text.size(); it++) {
        ui8char = static_cast<uint8_t>(text.at(it));
        it += ekg::utf8checksequence(ui8char, ui32char, utf8string, text, it);

        if (this->ft_bool_kerning && this->ft_uint_previous) {
            FT_Get_Kerning(this->ft_face, this->ft_uint_previous, ui32char, 0, &this->ft_vector_previous_char);
            x += static_cast<float>(this->ft_vector_previous_char.x >> 6);
        }

        char_data = this->allocated_char_data[ui32char];
        vertices.x = x + char_data.left;
        vertices.y = y + this->full_height - char_data.top;

        vertices.w = char_data.w;
        vertices.h = char_data.h;

        coordinates.x = char_data.x;
        coordinates.w = vertices.w / this->full_width;
        coordinates.h = vertices.h / this->full_height;

        this->allocator->vertex2f(vertices.x, vertices.y);
        this->allocator->vertex2f(vertices.x, vertices.y + vertices.h);
        this->allocator->vertex2f(vertices.x + vertices.w, vertices.y + vertices.h);
        this->allocator->vertex2f(vertices.x + vertices.w, vertices.y + vertices.h);
        this->allocator->vertex2f(vertices.x + vertices.w, vertices.y);
        this->allocator->vertex2f(vertices.x, vertices.y);

        this->allocator->coord2f(coordinates.x, coordinates.y);
        this->allocator->coord2f(coordinates.x, coordinates.y + coordinates.h);
        this->allocator->coord2f(coordinates.x + coordinates.w, coordinates.y + coordinates.h);
        this->allocator->coord2f(coordinates.x + coordinates.w, coordinates.y + coordinates.h);
        this->allocator->coord2f(coordinates.x + coordinates.w, coordinates.y);
        this->allocator->coord2f(coordinates.x, coordinates.y);

        x += char_data.wsize;
        this->ft_uint_previous = ui32char;
        data.factor += static_cast<int32_t>(x + ui32char);
    }

    this->allocator->bind_texture(this->texture);
    this->allocator->dispatch();
}

void ekg::draw::font_renderer::init() {

}

void ekg::draw::font_renderer::quit() {
    FT_Done_Face(this->ft_face);
}
