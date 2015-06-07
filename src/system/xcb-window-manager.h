//
// Created by Matthew Hinkle on 6/6/15.
//

#ifndef FORGE_XCB_WINDOW_MANAGER_H
#define FORGE_XCB_WINDOW_MANAGER_H

#ifdef WITH_XCB

#include <cassert>
#include <mutex>

#include "common/logger.h"
#include "include/xlib.h"
#include "include/xcb.h"
#include "include/gl.h"

#include "opengl-context.h"
#include "window-manager.h"

namespace forge {

    struct wm {
        unsigned default_screen;
        Display * display;
        xcb_connection_t * xcb;
        xcb_screen_t * screen;
        xcb_key_symbols_t * keysyms;
    };

    struct window {
        xcb_connection_t * xcb;
        xcb_screen_t * screen;
        xcb_window_t w;
        GLXFBConfig fbconfig;
    };

    struct glctx {
        Display * display;
        Drawable drawable;
        GLXContext native;
    };

    namespace xcbwm {
        static const Logger log("wm");
    }

}

#endif

#endif //FORGE_XCB_WINDOW_MANAGER_H
