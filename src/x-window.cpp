//
// Created by Matthew Hinkle on 5/29/15.
//

#if defined (FORGE_WINDOW_X11) && (FORGE_WINDOW_X11)

#include "x-window.h"

using namespace forge;

const Logger XWindow::log("XWindow");

Display * XWindow::open_display() const {
    int event = 0;
    int error = 0;
    int major = XkbMajorVersion;
    int minor = XkbMinorVersion;
    int reason = 0;
    Display * d = XkbOpenDisplay(NULL, &event, &error, &major, &minor, &reason);
    switch(reason) {
        case XkbOD_Success: return d;
        case XkbOD_BadLibraryVersion: log.error("Failed to initialize Xkb: version incompatibility between compile and run-time Xlib/Xkb libraries.\n"); break;
        case XkbOD_ConnectionRefused: log.error("Failed to initialize Xkb: connection refused when attempting to open display.\n"); break;
        case XkbOD_BadServerVersion: log.error("Failed to initialize Xkb: incompatible Xkb extension version between library and server.\n"); break;
        case XkbOD_NonXkbServer: log.warn("Failed to initialize Xkb: server does not support Xkb.\n"); break;
        default: log.warn("Unknown XkbOpenDisplay reason [id = %d] encountered.\n", reason); break;
    }

    log.warn("Falling back to core Xlib keyboard implemenation.\n");
    return XOpenDisplay(nullptr);
}

XVisualInfo * XWindow::get_visual_info() const {
    static int VI_ATTR[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_DEPTH_SIZE, 32,
            None
    };

    return glXChooseVisual(this->display, this->screen, VI_ATTR);
}

#endif

