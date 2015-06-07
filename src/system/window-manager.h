//
// Created by Matthew Hinkle on 6/1/15.
//

#ifndef FORGE_SYSTEM_WINDOW_MANAGER_H
#define FORGE_SYSTEM_WINDOW_MANAGER_H

/**
 * System dependent window interface.
 */

namespace forge {

    struct wm;      /* window manager forward declaration */
    struct window;  /* window forward declaration */
    struct glctx;   /* opengl context forward declaration */

    /**
     * Window manager interface
     */
    enum {
        WM_OPTION_NIL = 0,
        WM_OPTION_THREADS = 1 << 0,
        WM_OPTION_DEFAULT = WM_OPTION_THREADS
    };

    enum {
        WINDOW_OPTION_NIL = 0,
        WINDOW_OPTION_DOUBLE_BUFFERED = 1 << 0,
        WINDOW_OPTION_FULLSCREEN = 1 << 1,
        WINDOW_OPTION_HIDDEN = 1 << 2,
        WINDOW_OPTION_DEFAULT = WINDOW_OPTION_DOUBLE_BUFFERED
    };

    static const unsigned WINDOW_MAX_NAME_SIZE = 256;

    extern void init_wm(struct wm *, unsigned wmoptions);
    extern void destroy_wm(struct wm *);

    extern void wm_init_glctx(struct wm *, struct window * w, struct glctx *, unsigned ctxoptions);
    extern void wm_destroy_glctx(struct wm *, struct glctx *);

    extern void wm_init_window(struct wm *,
                               struct window *,
                               const char * name,
                               int x, int y, int w, int h,
                               unsigned windowoptions);
    extern void wm_destroy_window(struct wm *, struct window *);

    /**
     * Window interface
     */
    extern const char * window_get_name(const struct window * w);
    extern const int * window_get_bounds(const struct window * w);
    extern bool window_is_fullscreen(const struct window * w);

    extern void window_set_name(struct window *, const char * name);
    extern void window_set_bounds(struct window *, int x, int y, int width, int height);
    extern void window_set_fullscreen(struct window *, bool fullscreen);

    extern void window_map(struct window *);
}

#endif //FORGE_SYSTEM_WINDOW_MANAGER_H
