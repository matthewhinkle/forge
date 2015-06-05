//
// Created by Matthew Hinkle on 6/1/15.
//

#ifndef FORGE_X_WINDOW_MANAGER_H
#define FORGE_X_WINDOW_MANAGER_H

#ifdef WITH_XLIB

#include <cassert>
#include <mutex>

#include "common/logger.h"
#include "include/xlib.h"
#include "include/xkb.h"
#include "include/gl.h"

#include "window-manager.h"

namespace forge {

    struct wm {
        int default_screen;
        Display * display;
    };

    struct window {
        Display * display;
        Window w;
        Colormap colormap;
        XVisualInfo * vinfo;
    };

    namespace xwm {
        static const Logger log("wm");
    }

}

#endif

#endif //FORGE_X_WINDOW_MANAGER_H