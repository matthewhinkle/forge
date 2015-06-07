//
// Created by Matthew Hinkle on 6/6/15.
//

#ifdef WITH_XCB

#include "xcb-window-manager.h"

using forge::xcbwm::log;

namespace forge {

    std::once_flag one_time_init_flag;
    static void one_time_init(unsigned options) {
        if(WM_OPTION_THREADS & options) {
            if(!XInitThreads()) {
                log.warn("Failed to initialize X11 with threads.  This is expected on a single core.\n");
            }
        }
    }

    static xcb_screen_t * get_screen(xcb_connection_t * xcb, unsigned default_screen) {
        xcb_screen_iterator_t i = xcb_setup_roots_iterator(xcb_get_setup(xcb));
        for(int n = default_screen; i.rem && n > 0; n--, xcb_screen_next(&i));
        return i.data;
    }

    extern void init_wm(struct wm * wm, unsigned options) {
        std::call_once(one_time_init_flag, one_time_init, options);

        assert(wm);

        Display * d = XOpenDisplay(NULL);
        assert(d);

        unsigned default_screen = XDefaultScreen(d);

        xcb_connection_t * xcb = XGetXCBConnection(d);
        assert(xcb);

        XSetEventQueueOwner(d, XCBOwnsEventQueue);

        xcb_screen_t * screen = get_screen(xcb, default_screen);
        assert(screen);

        xcb_key_symbols_t * keysyms = xcb_key_symbols_alloc(xcb);
        assert(keysyms);

        wm->default_screen = default_screen;
        wm->display = d;
        wm->xcb = xcb;
        wm->screen = screen;
        wm->keysyms = keysyms;
    }

    extern void destroy_wm(struct wm * wm) {
        if(!wm) return;

        if(wm->display) {
            if(wm->xcb) {
                if(wm->keysyms) {
                    xcb_key_symbols_free(wm->keysyms);
                    wm->keysyms = NULL;
                }
                wm->xcb = NULL;
            }
            XCloseDisplay(wm->display);
            wm->display = NULL;
        }
    }

    extern void wm_init_glctx(struct wm * wm, struct window * w, struct glctx * ctx, unsigned ctxoptions) {
        assert(wm);
        assert(w);
        assert(ctx);

        GLXContext c = glXCreateNewContext(wm->display, w->fbconfig, GLX_RGBA_TYPE, NULL, True);

        assert(c);

        ctx->display = wm->display;
        ctx->drawable = w->w;
        ctx->native = c;
    }

    extern void wm_destroy_glctx(struct wm * wm, struct glctx * ctx) {
        if(!ctx) return;

        if(ctx->display && ctx->native) {
            glXDestroyContext(ctx->display, ctx->native);
            ctx->native = 0;
            ctx->display = 0;
        }
    }

    extern void * glctx_get_native(const struct glctx * ctx) {
        assert(ctx);

        return ctx->native;
    }

    extern void glctx_make_current(struct glctx * ctx) {
        assert(ctx);

        glXMakeContextCurrent(ctx->display, ctx->drawable, ctx->drawable, ctx->native);
    }

    extern void glctx_swap_buffers(struct glctx * ctx) {
        assert(ctx);

        glXSwapBuffers(ctx->display, ctx->drawable);
    }

    static const uint32_t EVENT_MASK = XCB_EVENT_MASK_EXPOSURE
                                       | XCB_EVENT_MASK_KEY_PRESS
                                       | XCB_EVENT_MASK_KEY_RELEASE
                                       | XCB_EVENT_MASK_BUTTON_PRESS
                                       | XCB_EVENT_MASK_BUTTON_RELEASE
                                       | XCB_EVENT_MASK_BUTTON_MOTION;
    static const uint32_t VALUE_MASK = XCB_CW_EVENT_MASK
                                       | XCB_CW_COLORMAP;

    static void _set_name(xcb_connection_t * xcb, xcb_window_t window, const char * name) {
        assert(xcb);
        assert(window);
        assert(name);

        xcb_change_property(xcb,
                            XCB_PROP_MODE_REPLACE,
                            window,
                            XCB_ATOM_WM_NAME,
                            XCB_ATOM_STRING,
                            8, /* 8-bit data format size */
                            WINDOW_MAX_NAME_SIZE,
                            name);
    }

    static const char _NET_WM_STATE[] = "_NET_WM_STATE";
    static const char _NET_WM_STATE_FULLSCREEN[] = "_NET_WM_STATE_FULLSCREEN";

    extern void wm_init_window(struct wm * wm,
                               struct window * w,
                               const char * name,
                               int x, int y, int width, int height,
                               unsigned options) {
        assert(wm);
        assert(w);
        assert(name);

        const int fbattr[] = {
                GLX_DOUBLEBUFFER, WINDOW_OPTION_DOUBLE_BUFFERED & options ? True : False,
                None
        };

        int fbcount = 0;
        const GLXFBConfig * fbconfigs = glXChooseFBConfig(wm->display, wm->default_screen, fbattr, &fbcount);
        assert(fbconfigs);
        assert(fbcount > 0);

        int visual_id = 0;
        glXGetFBConfigAttrib(wm->display, *fbconfigs, GLX_VISUAL_ID, &visual_id);

        xcb_colormap_t cmap = xcb_generate_id(wm->xcb);
        xcb_create_colormap(wm->xcb, XCB_COLORMAP_ALLOC_NONE, cmap, wm->screen->root, visual_id);

        if(WINDOW_OPTION_FULLSCREEN & options) {
            x = 0;
            y = 0;
            width = wm->screen->width_in_pixels;
            height = wm->screen->height_in_pixels;
        }

        const unsigned values[] = { EVENT_MASK, cmap, None };
        xcb_window_t window = xcb_generate_id(wm->xcb);
        xcb_create_window(wm->xcb,
                          XCB_COPY_FROM_PARENT,
                          window,
                          wm->screen->root,
                          x, y, width, height,
                          0, /* no border */
                          XCB_WINDOW_CLASS_INPUT_OUTPUT,
                          visual_id,
                          VALUE_MASK,
                          values);

        _set_name(wm->xcb, window, name);

        if(!(WINDOW_OPTION_HIDDEN & options)) {
            xcb_map_window(wm->xcb, window);

            if(WINDOW_OPTION_FULLSCREEN & options) {

                xcb_intern_atom_cookie_t state = xcb_intern_atom(wm->xcb, 0, strlen(_NET_WM_STATE), _NET_WM_STATE);
                xcb_intern_atom_reply_t * sreply = xcb_intern_atom_reply(wm->xcb, state, NULL);

                xcb_intern_atom_cookie_t fs = xcb_intern_atom(wm->xcb, 0, strlen(_NET_WM_STATE_FULLSCREEN), _NET_WM_STATE_FULLSCREEN);
                xcb_intern_atom_reply_t * fsreply = xcb_intern_atom_reply(wm->xcb, fs, NULL);

                xcb_change_property(w->xcb,
                                    XCB_PROP_MODE_REPLACE,
                                    window,
                                    sreply->atom,
                                    XCB_ATOM_ATOM,
                                    32, /* 32-bit data format size */
                                    1,
                                    &fsreply->atom);
            }
        }

        xcb_flush(wm->xcb);

        w->xcb = wm->xcb;
        w->screen = wm->screen;
        w->w = window;
        w->fbconfig = *fbconfigs;
    }

    extern void wm_destroy_window(struct wm * wm, struct window * w) {
        if(!w) return;

        if(w->xcb && w->w) {
            xcb_destroy_window(w->xcb, w->w);
            w->w = 0;
            w->screen = NULL;
            w->xcb = NULL;
        }
    }

    extern void window_set_name(struct window * w, const char * name) {
        assert(w);

        _set_name(w->xcb, w->w, name);
        xcb_flush(w->xcb);
    }

    extern void window_set_bounds(struct window * w, int x, int y, int width, int height) {
        assert(width);

        assert(width > 0);
        assert(height > 0);

        const unsigned values[] = {
                static_cast<unsigned>(x),
                static_cast<unsigned>(y),
                static_cast<unsigned>(width),
                static_cast<unsigned>(height)
        };

        xcb_configure_window(w->xcb,
                             w->w,
                             XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                             values);
        xcb_flush(w->xcb);
    }

    enum {
        _NET_WM_STATE_REMOVE = 0,
        _NET_WM_STATE_ADD = 1,
        _NET_WM_STATE_TOGGLE = 2
    };

    extern void window_set_fullscreen(struct window * w, bool fullscreen) {
        assert(w);

        xcb_intern_atom_cookie_t state = xcb_intern_atom(w->xcb, 0, strlen(_NET_WM_STATE), _NET_WM_STATE);
        xcb_intern_atom_reply_t * sreply = xcb_intern_atom_reply(w->xcb, state, NULL);

        xcb_intern_atom_cookie_t fs = xcb_intern_atom(w->xcb, 0, strlen(_NET_WM_STATE_FULLSCREEN), _NET_WM_STATE_FULLSCREEN);
        xcb_intern_atom_reply_t * fsreply = xcb_intern_atom_reply(w->xcb, fs, NULL);

        xcb_client_message_event_t e;
        e.response_type = XCB_CLIENT_MESSAGE;
        e.window = w->w;
        e.type = sreply->atom,
        e.format = 32,
        e.data.data32[0] = fullscreen ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
        e.data.data32[1] = fsreply->atom,
        e.data.data32[2] = XCB_ATOM_NONE,
        e.data.data32[3] = 0;
        e.data.data32[4] = 0;

        xcb_send_event(w->xcb,
                       1,
                       w->w,
                       XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY,
                       reinterpret_cast<const char *>(&e));
        xcb_flush(w->xcb);
    }

    extern void window_map(struct window * w) {
        assert(w);

        xcb_map_window(w->xcb, w->w);
        xcb_flush(w->xcb);
    }
}

#endif
