//
// Created by Matthew Hinkle on 6/6/15.
//

#ifndef FORGE_COCOA_WINDOW_MANAGER_H
#define FORGE_COCOA_WINDOW_MANAGER_H

#ifdef WITH_COCOA

#include <cassert>

#include "common/logger.h"
#include "include/gl.h"

#include "opengl-context.h"
#include "window-manager.h"

namespace forge {

    struct wm { };

    struct window {
        const void * native;
        int bounds[4];
        bool fullscreen;
    };

    struct glctx {
        const void * native;
    };

    namespace cocoawm {
        static const Logger log("wm");
    }

}

#endif

#endif //FORGE_COCOA_WINDOW_MANAGER_H
