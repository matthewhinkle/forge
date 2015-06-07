//
// Created by Matthew Hinkle on 6/4/15.
//

#ifndef FORGE_SYSTEM_OPENGL_CONTEXT_H
#define FORGE_SYSTEM_OPENGL_CONTEXT_H

namespace forge {
    struct glctx;

    enum {
        GLCTX_OPTION_NIL = 0,
        GLCTX_OPTION_DOUBLE_BUFFERED = 1 << 0,
        GLCTX_OPTION_DEFAULT = GLCTX_OPTION_DOUBLE_BUFFERED
    };

    /**
     * Get the platform dependent OpenGL context.
     */
    extern void * glctx_get_native(const struct glctx *);

    extern void glctx_make_current(struct glctx *);

    extern void glctx_swap_buffers(struct glctx *);
}

#endif //FORGE_SYSTEM_OPENGL_CONTEXT_H
