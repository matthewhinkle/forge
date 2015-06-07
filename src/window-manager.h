//
// Created by Matthew Hinkle on 6/7/15.
//

#ifndef FORGE_WINDOW_MANAGER_H
#define FORGE_WINDOW_MANAGER_H

#include <string>

#include "math/rect.h"
#include "math/vec.h"
#include "system/opengl-context.h"
#include "system/window-manager-incl.h"

namespace forge {

    class OpenGLContext {
    public:
        OpenGLContext(const struct glctx & ctx);
        OpenGLContext(const OpenGLContext & ctx);
        OpenGLContext(OpenGLContext && ctx);
        OpenGLContext & operator=(const OpenGLContext & ctx);
        OpenGLContext & operator=(OpenGLContext && ctx);

        struct glctx * get_context();

        void make_current();
        void swap_buffers();
    private:
        struct glctx ctx;
    };

    class Window {
    public:
        Window(const struct window & w);
        Window(const Window & w);
        Window(Window && w);
        Window & operator=(const Window & w);
        Window & operator=(Window && w);
        ~Window();

        const std::string & get_name() const;
        const rect & get_bounds() const;
        bool is_fullscreen() const;

        struct window * get_window();

        void set_name(const std::string & name = default_name());
        void set_bounds(const rect & bounds = default_bounds());
        void set_fullscreen(bool fullscreen = true);

        static std::string default_name() { return "Forge"; }
        static rect default_bounds() { return rect(0, 0, 1024, 768); }

    private:
        std::string name;
        rect bounds;
        bool fullscreen;

        struct window w;
    };

    class WindowManager {
    public:
        WindowManager(bool threaded = true);
        WindowManager(WindowManager && w);
        WindowManager & operator=(WindowManager && w);
        ~WindowManager();

        WindowManager(const WindowManager &) = delete;
        WindowManager & operator=(const WindowManager &) = delete;

        Window make_window(const std::string & name = Window::default_name(),
                           const rect & bounds = Window::default_bounds(),
                           bool fullscreen = false);

        OpenGLContext make_opengl_context(Window * w);
    private:
        struct wm wm;
    };

}

#endif //FORGE_WINDOW_MANAGER_H
