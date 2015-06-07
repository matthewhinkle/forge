//
// Created by Matthew Hinkle on 6/7/15.
//

#include "system/window-manager.h"
#include "window-manager.h"

using namespace forge;

OpenGLContext::OpenGLContext(const struct glctx & ctx) : ctx(ctx) { }

OpenGLContext::OpenGLContext(const OpenGLContext & ctx) : ctx(ctx.ctx) { }

OpenGLContext::OpenGLContext(OpenGLContext && ctx) : ctx(std::move(ctx.ctx)) { }

OpenGLContext & OpenGLContext::operator=(const OpenGLContext & ctx) {
    this->ctx = ctx.ctx;
    return *this;
}

OpenGLContext & OpenGLContext::operator=(OpenGLContext && ctx) {
    this->ctx = std::move(ctx.ctx);
    return *this;
}

struct glctx * OpenGLContext::get_context() {
    return &this->ctx;
}

void OpenGLContext::make_current() {
    glctx_make_current(&this->ctx);
}

void OpenGLContext::swap_buffers() {
    glctx_swap_buffers(&this->ctx);
}

Window::Window(const struct window & w)
        : w(w), name(window_get_name(&w)), bounds(0, 0, 0, 0), fullscreen(window_is_fullscreen(&w)) {

    const int * window_bounds = window_get_bounds(&w);
    assert(window_bounds);

    this->bounds = rect(static_cast<float>(window_bounds[0]),
                        static_cast<float>(window_bounds[1]),
                        static_cast<float>(window_bounds[2]),
                        static_cast<float>(window_bounds[3]));
}

Window::Window(const Window & w) : w(w.w), name(w.name), bounds(w.bounds), fullscreen(w.fullscreen) { }

Window::Window(Window && w) : w(std::move(w.w)), name(std::move(w.name)), bounds(std::move(w.bounds)), fullscreen(w.fullscreen) { }

Window & Window::operator=(const Window & w) {
    this->w = w.w;
    this->name = w.name;
    this->bounds = w.bounds;
    this->fullscreen = w.fullscreen;
    return *this;
}

Window & Window::operator=(Window && w) {
    this->w = std::move(w.w);
    this->name = std::move(w.name);
    this->bounds = std::move(w.bounds);
    this->fullscreen = w.fullscreen;
    return *this;
}

Window::~Window() { }

const std::string & Window::get_name() const {
    return this->name;
}

const rect & Window::get_bounds() const {
    return this->bounds;
}

bool Window::is_fullscreen() const {
    return this->fullscreen;
}

struct window * Window::get_window() {
    return &this->w;
}

void Window::set_name(const std::string & name) {
    window_set_name(&this->w, name.c_str());

    this->name = name;
}

void Window::set_bounds(const rect & bounds) {
    window_set_bounds(&this->w, bounds.x, bounds.y, bounds.width, bounds.height);

    this->bounds = bounds;
}

void Window::set_fullscreen(bool fullscreen) {
    window_set_fullscreen(&this->w, fullscreen);

    this->fullscreen = fullscreen;
}

WindowManager::WindowManager(bool threaded) {
    init_wm(&this->wm, threaded ? WM_OPTION_THREADS : WM_OPTION_NIL);
}

WindowManager::WindowManager(WindowManager && w) : wm(std::move(w.wm)) { }

WindowManager & WindowManager::operator=(WindowManager && w) {
    this->wm = std::move(w.wm);
    return *this;
}

WindowManager::~WindowManager() {
    destroy_wm(&this->wm);
}

Window WindowManager::make_window(const std::string & name, const rect & bounds, bool fullscreen) {
    struct window w;
    wm_init_window(&this->wm,
                   &w,
                   name.c_str(),
                   bounds.x, bounds.y, bounds.width, bounds.height,
                   WINDOW_OPTION_DOUBLE_BUFFERED | (fullscreen ? WINDOW_OPTION_FULLSCREEN : 0));
    return Window(w);
}

OpenGLContext WindowManager::make_opengl_context(Window * w) {
    struct glctx ctx;
    wm_init_glctx(&this->wm, w->get_window(), &ctx, GLCTX_OPTION_DEFAULT);
    return OpenGLContext(ctx);
}
