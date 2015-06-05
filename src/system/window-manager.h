//
// Created by Matthew Hinkle on 6/1/15.
//

#ifndef FORGE_WINDOW_MANAGER_H
#define FORGE_WINDOW_MANAGER_H

/**
 * System dependent window interface.
 */

namespace forge {

    struct wm;      /* window manager forward declaration */
    struct window;  /* window forward declaration */

    /**
     * Window manager interface
     */
    enum wmoptions {
        WM_OPTION_NIL = 0,
        WM_OPTION_THREADS = 1 << 0,
        WM_OPTION_DEFAULT = WM_OPTION_THREADS
    };

    enum windowoptions {
        WINDOW_OPTION_NIL = 0,
        WINDOW_OPTION_DOUBLE_BUFFERED = 1 << 0,
        WINDOW_OPTION_FULLSCREEN = 1 << 1,
        WINDOW_OPTION_HIDDEN = 1 << 2,
        WINDOW_OPTION_DEFAULT = WINDOW_OPTION_DOUBLE_BUFFERED
    };

    extern void init_wm(struct wm *, enum wmoptions);
    extern void destroy_wm(struct wm *);

    extern void wm_init_window(struct wm *,
                               struct window *,
                               const char * name,
                               int x, int y, int w, int h,
                               enum windowoptions);
    extern void wm_destroy_window(struct wm *, struct window *);

    /**
     * Window interface
     */
    extern void window_set_name(struct window *, const char * name);
    extern void window_set_bounds(struct window *, int x, int y, int w, int h);
    extern void window_set_fullscreen(struct window *, bool fullscreen);

    extern void window_map(struct window *);

}

#endif //FORGE_WINDOW_MANAGER_H
