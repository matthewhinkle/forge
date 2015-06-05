//
// Created by Matthew Hinkle on 6/1/15.
//

#ifdef WITH_XLIB

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "x-window-manager.h"

using forge::xwm::log;

namespace forge {

    static const int PROPERTY_FORMAT_8 = 8;
    static const int PROPERTY_FORMAT_32 = 32;

    std::once_flag one_time_init_flag;
    static void one_time_init(struct wm * wm, enum wmoptions options) {
        if(WM_OPTION_THREADS & options) {
            if(!XInitThreads()) {
                log.warn("Failed to initialize X11 with threads.  This is expected on a single core.\n");
            }
        }
    }

    static Display * open_display()
    {
        int event = 0;
        int error = 0;
        int major = XkbMajorVersion;
        int minor = XkbMinorVersion;
        int reason = 0;
        Display * d = XkbOpenDisplay(nullptr, &event, &error, &major, &minor, &reason);
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

    extern void init_wm(struct wm * wm, enum wmoptions options) {
        std::call_once(one_time_init_flag, one_time_init, wm, options);

        assert(wm);

        Display * d = XOpenDisplay(NULL);
        int default_screen = XDefaultScreen(d);

        assert(d);

        wm->display = d;
        wm->default_screen = default_screen;
    }

    extern void destroy_wm(struct wm * wm) {
        if(!wm) return;

        XCloseDisplay(wm->display);
        wm->display = nullptr;
    }

    static const unsigned long EVENT_MASK = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask | PointerMotionHintMask;
    static const unsigned long VALUE_MASK =  CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

    static const int DEPTH_SIZE = 32;

    static int VI_ATTR_SINGLE_BUFFERED[] = {
            GLX_RGBA,
            GLX_DEPTH_SIZE, DEPTH_SIZE,
            None
    };

    static int VI_ATTR[] = {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_DEPTH_SIZE, DEPTH_SIZE,
            None
    };

    static const unsigned char HINTS_FULLSCREEN[] = {
            2,		/* flags: specify we are changing the decorations field (idx 2) */
            0,		/* functions: unused for fullscreen */
            0,		/* decorations: remove decorations */
            0,		/* inputMode: unused */
            0		/* status: unused */
    };

    static const unsigned char HINTS_WINDOWED[] = {
            2,		/* flags: specify we are changing the decorations field (idx 2) */
            0,		/* functions: unused for windowed */
            1,		/* decorations: add decorations */
            0,		/* inputMode: unused */
            0		/* status: unused */
    };

    static void _set_fullscreen(struct window * w) {
        assert(w);

        Atom prop = XInternAtom(w->display, "_MOTIF_WM_HINTS", True);
        assert(prop);

        static const int hint_count = sizeof(HINTS_FULLSCREEN) / sizeof(*HINTS_FULLSCREEN);

        XChangeProperty(w->display,
                        w->w,
                        prop,
                        prop,
                        PROPERTY_FORMAT_32,
                        PropModeReplace,
                        HINTS_FULLSCREEN,
                        hint_count);
    }

    extern void wm_init_window(struct wm * wm,
                               struct window * w,
                               const char * name,
                               int x, int y, int width, int height,
                               enum windowoptions options) {
        assert(wm);
        assert(w);
        assert(name);

        const bool is_double_buffered = false;//WINDOW_OPTION_DOUBLE_BUFFERED & options;
        XVisualInfo * vinfo = glXChooseVisual(wm->display, wm->default_screen, is_double_buffered ? VI_ATTR : VI_ATTR_SINGLE_BUFFERED);
        assert(vinfo);

        Window root = XRootWindow(wm->display, vinfo->screen);
        assert(root);

        if(WINDOW_OPTION_FULLSCREEN & options) {
            const Screen * s = XScreenOfDisplay(wm->display, wm->default_screen);
            x = 0;
            y = 0;
            width = s->width;
            height = s->height;
        }

        Colormap cmap = XCreateColormap(wm->display, root, vinfo->visual, AllocNone);
        XSetWindowAttributes attr;
        attr.background_pixel = 0;
        attr.border_pixel = 0;
        attr.colormap = cmap;
        attr.event_mask = EVENT_MASK;

        ::Window window = XCreateWindow(
                wm->display,
                root,
                x, y, static_cast<unsigned>(width), static_cast<unsigned>(height),
                0,  /* no border */
                vinfo->depth,
                InputOutput,
                vinfo->visual,
                VALUE_MASK,
                &attr
        );
        assert(window);

        w->display = wm->display;
        w->w = window;
        w->colormap = cmap;
        w->vinfo = vinfo;

        if(WINDOW_OPTION_FULLSCREEN & options) _set_fullscreen(w);
        if(name) window_set_name(w, name);
        if(!(WINDOW_OPTION_HIDDEN & options)) XMapWindow(wm->display, window);

        XFlush(wm->display);
    }

    extern void wm_destroy_window(struct wm * wm, struct window * w) {
        assert(wm);
        assert(w);

        if(w) {
            XFreeColormap(wm->display, w->colormap);
            XDestroyWindow(wm->display, w->w);

            w->colormap = 0;
            w->w = 0;
        }
    }

    static const int MAX_NAME_LENGTH = 1024;

    extern void window_set_name(struct window * w, const char * name) {
        assert(w);
        assert(name);

        const Atom prop = XInternAtom(w->display, "WM_NAME", True);
        const Atom type = XInternAtom(w->display, "STRING", True);

        assert(prop);
        assert(type);

        char * name_copy = const_cast<char *>(name);

        XChangeProperty(w->display,
                        w->w,
                        prop,
                        type,
                        PROPERTY_FORMAT_8,
                        PropModeReplace,
                        reinterpret_cast<unsigned char *>(name_copy),
                        MAX_NAME_LENGTH);
        XFlush(w->display);
    }

    enum {
        _NET_WM_STATE_REMOVE = 0,
        _NET_WM_STATE_ADD = 1,
        _NET_WM_STATE_TOGGLE = 2
    };

    extern void window_set_bounds(struct window * w, int x, int y, int width, int height) {
        assert(w);
        assert(width >= 0);
        assert(height >= 0);

        XMoveResizeWindow(w->display, w->w, x, y, static_cast<unsigned>(width), static_cast<unsigned>(height));
        XFlush(w->display);
    }

    extern void window_set_fullscreen(struct window * w, bool fullscreen) {
        assert(w);

        Atom state = XInternAtom(w->display, "_NET_WM_STATE", True);
        Atom fs = XInternAtom(w->display, "_NET_WM_STATE_FULLSCREEN", True);

        assert(state);
        assert(fs);

        XClientMessageEvent e;
        e.type = ClientMessage;
        e.display = w->display;
        e.window = w->w;
        e.message_type = state;
        e.format = 32;
        e.data.l[0] = fullscreen ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
        e.data.l[1] = fs;
        e.data.l[2] = 0;
        e.data.l[3] = 0;
        e.data.l[4] = 0;

        XSendEvent(w->display,
                   w->w,
                   True,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   reinterpret_cast<XEvent *>(&e));
        XFlush(w->display);
    }

    extern void window_map(struct window * w) {
        assert(w);

        XMapWindow(w->display, w->w);
        XFlush(w->display);
    }

}

#endif
