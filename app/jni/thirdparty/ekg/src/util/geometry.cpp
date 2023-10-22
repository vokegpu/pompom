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

#include "ekg/ekg.hpp"

int32_t ekg::display::width {};
int32_t ekg::display::height {};

float ekg::display::dt {};
double ekg::pi {3.141592653589793238462643383279502884};

bool ekg::rect_collide_rect(const ekg::rect &rect_a, const ekg::rect &rect_b) {
    return rect_a.x < rect_b.x + rect_b.w && rect_a.x + rect_a.w > rect_b.x &&
           rect_a.y < rect_b.y + rect_b.h && rect_a.x + rect_a.w > rect_b.y;
}

void ekg::orthographic2d(float *matrix, float left, float right, float bottom, float top) {
    const float depth_near = -1.0f;
    const float depth_far = 1.0f;
    const float depth_inv = 1.0f / (depth_far - depth_near);
    const float y_inv = 1.0f / (top - bottom);
    const float x_inv = 1.0f / (right - left);

    matrix[0] = 2.0f * x_inv;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 2.0f * y_inv;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = -2.0f * depth_inv;
    matrix[11] = 0.0f;

    matrix[12] = -(right + left) * x_inv;
    matrix[13] = -(top + bottom) * y_inv;
    matrix[14] = -(depth_far + depth_near) * depth_inv;
    matrix[15] = 1.0f;
}

bool ekg::rect_collide_vec_precisely(const ekg::rect &rect, const ekg::vec4 &vec) {
    return vec.x >= rect.x && vec.x <= rect.x + rect.w && vec.y >= rect.y && vec.y <= rect.y + rect.h;
}

bool ekg::rect_collide_vec(const ekg::rect &rect, const ekg::vec4 &vec) {
    return vec.x > rect.x && vec.x < rect.x + rect.w && vec.y > rect.y && vec.y < rect.y + rect.h;
}

void ekg::set_dock_scaled(const ekg::rect &rect, const ekg::vec2 &offset, ekg::docker &docker) {
    docker.rect = rect;

    docker.left.x = rect.x;
    docker.left.y = rect.y;
    docker.left.w = offset.x;
    docker.left.h = rect.h;

    docker.right.w = offset.x;
    docker.right.h = rect.h;
    docker.right.x = rect.x + rect.w - offset.x;
    docker.right.y = rect.y;

    docker.top.x = rect.x;
    docker.top.y = rect.y;
    docker.top.w = rect.w;
    docker.top.h = offset.y;

    docker.bottom.w = rect.w;
    docker.bottom.h = offset.y;
    docker.bottom.x = rect.x;
    docker.bottom.y = rect.y + rect.h - offset.y;

    float dx = offset.x / 2;
    float dy = offset.y / 2;

    docker.center.x = rect.x + (rect.w / 2) - dx;
    docker.center.y = rect.y + (rect.h / 2) - dy;
    docker.center.w = dx;
    docker.center.h = dy;
}

int32_t ekg::find_collide_dock(ekg::docker &docker, uint16_t flags, const ekg::vec4 &vec) {
    if (ekg::bitwise::contains(flags, ekg::dock::full) && ekg::rect_collide_vec(docker.rect, vec)) {
        return ekg::dock::full;
    }

    uint16_t collided {ekg::dock::none};

    collided = ekg::bitwise::contains(flags, ekg::dock::left) && ekg::rect_collide_vec(docker.left, vec) ? ekg::bitwise::add(collided, ekg::dock::left) : collided;
    collided = ekg::bitwise::contains(flags, ekg::dock::right) && ekg::rect_collide_vec(docker.right, vec) ? ekg::bitwise::add(collided, ekg::dock::right) : collided;
    collided = ekg::bitwise::contains(flags, ekg::dock::top) && ekg::rect_collide_vec(docker.top, vec) ? ekg::bitwise::add(collided, ekg::dock::top) : collided;
    collided = ekg::bitwise::contains(flags, ekg::dock::bottom) && ekg::rect_collide_vec(docker.bottom, vec) ? ekg::bitwise::add(collided, ekg::dock::bottom) : collided;
    collided = ekg::bitwise::contains(flags, ekg::dock::center) && ekg::rect_collide_vec(docker.center, vec) ? ekg::bitwise::add(collided, ekg::dock::center) : collided;

    return collided;
}

void ekg::set_rect_clamped(ekg::rect &rect, float min_size) {
    rect.x = ekg::min(rect.x, 0.0f);
    rect.y = ekg::min(rect.y, 0.0f);
    rect.w = ekg::min(rect.w, min_size);
    rect.h = ekg::min(rect.h, min_size);
}

int32_t ekg::min(int32_t v, int32_t m) {
    return v < m ? m : v;
}

int32_t ekg::max(int32_t v, int32_t m) {
    return v > m ? m : v;
}

uint64_t ekg::min(uint64_t v, uint64_t m) {
    return v < m ? m : v;
}

uint64_t ekg::max(uint64_t v, uint64_t m) {
    return v > m ? m : v;
}

int64_t ekg::min(int64_t v, int64_t m) {
    return v < m ? m : v;
}

int64_t ekg::max(int64_t v, int64_t m) {
    return v > m ? m : v;
}

float ekg::clamp(float value, float min, float max) {
    return value < min ? min : (value > max ? max : value);
}

float ekg::min(float a, float b) {
    return a < b ? b : a;
}

float ekg::max(float a, float b) {
    return a > b ? b : a;
}

float ekg::find_min_offset(float text_width, float offset) {
    /* 
     * Use initial offset to get the min possible offset.
     * Initial offset value sometime is font height divided by 2.
     */
    float full_rect {text_width + offset};
    return static_cast<float>(static_cast<int32_t>((full_rect * 0.5f) - (text_width * 0.5f)));
}

bool ekg::operator==(const ekg::rect &l, const ekg::rect &r) {
    return ekg_equals_float(l.x, r.x) && ekg_equals_float(l.y, r.y) && ekg_equals_float(l.w, r.w) && ekg_equals_float(l.h, r.h);
}

bool ekg::operator!=(const ekg::rect &l, const ekg::rect &r) {
    return !(l == r);
}

bool ekg::operator==(const ekg::rect &l, const ekg::vec4 &r) {
    return ekg_equals_float(l.x, r.x) && ekg_equals_float(l.y, r.y) && ekg_equals_float(l.w, r.z) && ekg_equals_float(l.h, r.w);
}

bool ekg::operator!=(const ekg::rect &l, const ekg::vec4 &r) {
    return !(l == r);
}

ekg::rect ekg::operator-(const ekg::rect &l, const ekg::rect &r) {
    return {l.x - r.x, l.y - r.y, l.w, l.h};
}

ekg::rect ekg::operator+(const ekg::rect &l, const ekg::rect &r) {
    return {l.x + r.x, l.y + r.y, l.w, l.h};
}

ekg::rect ekg::operator-(const ekg::rect &l, const ekg::vec4 &r) {
    return {l.x - r.x, l.y - r.y, l.w, l.h};
}

ekg::rect ekg::operator+(const ekg::rect &l, const ekg::vec4 &r) {
    return {l.x + r.x, l.y + r.y, l.w, l.h};
}

ekg::vec4 ekg::operator-(const ekg::vec4 &l, const ekg::vec4 &r) {
    return {l.x - r.x, l.y - r.y, l.z, l.w};
}

ekg::vec4 ekg::operator+(const ekg::vec4 &l, const ekg::vec4 &r) {
    return {l.x + r.x, l.y + r.y, l.z, l.w};
}

ekg::vec4 ekg::operator-(const ekg::vec4 &l, const ekg::rect &r) {
    return {l.x - r.x, l.y - r.y, l.z, l.w};
}

ekg::vec4 ekg::operator+(const ekg::vec4 &l, const ekg::rect &r) {
    return {l.x + r.x, l.y + r.y, l.z, l.w};
}

ekg::vec2 ekg::operator/(const ekg::vec2 &l, float r) {
    return {l.x / r, l.y / r};
}

ekg::vec4 ekg::operator/(const ekg::vec4 &l, float r) {
    return {l.x / r, l.y / r, l.z / r, l.w / r};
}

float ekg::smooth(float duration, uint64_t ticks) {
    duration = static_cast<float>(ticks) / duration;
    return ekg::clamp(6 * powf(duration, 5) - (15 * powf(duration, 4)) + (10 * powf(duration, 3)), 0.0f, 1.0f);
}

float ekg::lerp(float a, float b, float dt) {
    return a + (b - a) * dt;
}

ekg::vec4 ekg::color(int32_t r, int32_t g, int32_t b, int32_t a) {
    return {static_cast<float>(r) / 255, static_cast<float>(g) / 255, static_cast<float>(b) / 255, static_cast<float>(a) / 255};
}