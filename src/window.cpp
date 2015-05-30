//
// Created by Matthew Hinkle on 5/26/15.
//
#if 0
#include "window.h"

using forge::Window;

Window::Window()
        :
        name(default_name()),
        position(default_position()),
        size(default_size()),
        is_fullscreen(false),
        w(frg_new_window()) { }

Window::Window(const Window::Window & w)
        :
        name(w.name),
        position(w.position),
        size(w.size),
        is_fullscreen(w.is_fullscreen),
        w(frg_clone_window(w.w)) { }

Window::Window(Window::Window && w)
        :
        name(std::move(w.name)),
        position(std::move(w.position)),
        size(std::move(w.size)),
        is_fullscreen(w.is_fullscreen),
        w(w.w) { }

Window::Window & operator=(const Window::Window & w) {
        this->name = w.name;
        this->position = w.position;
        this->size = w.size;
        this->is_fullscreen = w.is_fullscreen;
        this->w = frg_clone_window(w.w);

        return *this;
}

Window::Window & operator=(Window::Window && w) {
        this->name = std::move(w.name);
        this->position = std::move(w.position);
        this->size = std::move(w.size);
        this->is_fullscreen = w.is_fullscreen;
        this->w = w.w;

        return *this;
}

Window::~Window() {
        if(this->w) {
                frg_delete_window(this->w);
                this->w = nullptr;
        }
}

void Window::set_name(const std::string &name) {
        this->name = name;
        frg_window_set_name(this->w, this->name.c_str());
}

void Window::set_position(const vec2i & position) {
        this->position = position;
        frg_window_set_position(this->w, position.x, position.y);
}

void Window::set_size(const vec2i & size) {
        this->size = size;
        frg_window_set_size(this->w, size.x, size.y);
}
#endif