//
// Created by Matthew Hinkle on 6/6/15.
//

#ifdef WITH_COCOA

#include "cocoa-window-manager.h"

#import <Cocoa/Cocoa.h>

@interface FRGView : NSOpenGLView
@end

@implementation FRGView

- (id) init {
    return self = [super init];
};

- (id) initWithCoder:(NSCoder *)aDecoder {
    return self = [super initWithCoder:aDecoder];
}

- (id) initWithFrame:(NSRect)frame {
    return self = [super initWithFrame:frame];
}

- (id) initWithFrame:(NSRect)frame pixelFormat:(NSOpenGLPixelFormat *)format {
    return self = [super initWithFrame:frame pixelFormat:format];
}

- (BOOL) acceptsFirstResponder {
    return YES;
}

@end

namespace forge {

    extern void init_wm(struct wm * wm, unsigned wmoptions) { }
    extern void destroy_wm(struct wm * wm) { }

    extern void wm_init_glctx(struct wm * wm, struct window * w, struct glctx * ctx, unsigned ctxoptions) {
        assert(wm);
        assert(w);
        assert(ctx);

        const NSOpenGLPixelFormatAttribute pixel_fmt_attr[] = {
                NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
                NSOpenGLPFAAccelerated,
                NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
                NSOpenGLPFAColorSize, 24,
                NSOpenGLPFADepthSize, 32,
                GLCTX_OPTION_DOUBLE_BUFFERED ? NSOpenGLPFADoubleBuffer : 0,
                0
        };

        NSWindow * native = (__bridge NSWindow *)w->native;

        FRGView * view = [[FRGView alloc] initWithFrame:[native frame]
                                            pixelFormat:[[NSOpenGLPixelFormat alloc] initWithAttributes:pixel_fmt_attr]];

        [native setContentView:view];
        [native flushWindow];

        ctx->native = CFBridgingRetain(view);
    }

    extern void wm_destroy_glctx(struct wm * wm, struct glctx * ctx) {
        if(!ctx) return;

        if(ctx->native) {
            CFBridgingRelease(ctx->native);
            ctx->native = NULL;
        }
    }

    extern void * glctx_get_native(const struct glctx * ctx) {
        assert(ctx);

        return (__bridge void *)[(__bridge NSOpenGLView *)ctx->native openGLContext];
    }

    extern void glctx_make_current(struct glctx * ctx) {
        assert(ctx);

        [[(__bridge NSOpenGLView *)ctx->native openGLContext] makeCurrentContext];
    }

    extern void glctx_swap_buffers(struct glctx * ctx) {
        assert(ctx);

        [[(__bridge NSOpenGLView *)ctx->native openGLContext] flushBuffer];
    }

    const NSUInteger STYLE_MASK_FULLSCREEN = (NSBorderlessWindowMask| NSFullScreenWindowMask)
                                             & ~(NSTitledWindowMask
                                                 | NSClosableWindowMask
                                                 | NSMiniaturizableWindowMask
                                                 | NSResizableWindowMask
                                                 | NSTitledWindowMask);

    const NSUInteger STYLE_MASK_WINDOWED = (NSClosableWindowMask
                                            | NSMiniaturizableWindowMask
                                            | NSResizableWindowMask
                                            | NSTitledWindowMask)
                                           & ~(NSBorderlessWindowMask | NSFullScreenWindowMask);

    extern void wm_init_window(struct wm * wm,
                               struct window * w,
                               const char * name,
                               int x, int y, int width, int height,
                               unsigned windowoptions) {
        assert(w);

        NSUInteger styleMask = 0;
        NSRect contentRect;
        if(WINDOW_OPTION_FULLSCREEN & windowoptions) {
            styleMask = STYLE_MASK_FULLSCREEN;
            contentRect = [[NSScreen mainScreen] frame];
        } else {
            styleMask = STYLE_MASK_WINDOWED;
            contentRect = NSMakeRect(x, y, width, height);
        }

        NSWindow * native = [[NSWindow alloc] initWithContentRect:contentRect
                                                        styleMask:styleMask
                                                          backing:NSBackingStoreBuffered
                                                            defer:NO];

        [native setTitle:[NSString stringWithUTF8String:name]];

        [native setLevel:NSMainMenuWindowLevel + 1];
        if(!(WINDOW_OPTION_HIDDEN & windowoptions)) {
            [native makeKeyAndOrderFront:native];
        }

        w->native = CFBridgingRetain(native);
        w->bounds[0] = contentRect.origin.x;
        w->bounds[1] = contentRect.origin.y;
        w->bounds[2] = contentRect.size.width;
        w->bounds[3] = contentRect.size.height;
    }

    extern void wm_destroy_window(struct wm * wm, struct window * w) {
        if(!w) return;

        if(w->native) {
            CFBridgingRelease(w->native);
            w->native = NULL;
        }
    }

    extern const char * window_get_name(const struct window * w) {
        assert(w);

        return [[(__bridge NSWindow *)w->native title] cStringUsingEncoding:NSASCIIStringEncoding];
    }

    extern const int * window_get_bounds(const struct window * w) {
        assert(w);

        return w->bounds;
    }

    extern bool window_is_fullscreen(const struct window * w) {
        assert(w);

        return w->fullscreen;
    }

    extern void window_set_name(struct window * w, const char * name) {
        assert(w);

        [(__bridge NSWindow *)w->native setTitle:[NSString stringWithUTF8String:name]];
    }

    extern void window_set_bounds(struct window * w, int x, int y, int width, int height) {
        assert(w);

        const NSRect frame = NSMakeRect(x, y, width, height);
        [(__bridge NSWindow *)w->native setFrame:frame display:YES animate:YES];

        w->bounds[0] = x;
        w->bounds[1] = y;
        w->bounds[2] = width;
        w->bounds[3] = height;
    }

    extern void window_set_fullscreen(struct window * w, bool fullscreen) {
        assert(w);

        NSWindow * native = (__bridge NSWindow *)w->native;

        if(fullscreen) {
            [native setFrame:[[NSScreen mainScreen] frame] display:YES animate:YES];
            [native setStyleMask:STYLE_MASK_FULLSCREEN];
        } else {
            [native setStyleMask:STYLE_MASK_WINDOWED];
            window_set_bounds(w, w->bounds[0], w->bounds[1], w->bounds[2], w->bounds[3]);
        }

        w->fullscreen = fullscreen;
    }

    extern void window_map(struct window * w) {
        assert(w);

        NSWindow * native = (__bridge NSWindow *)w->native;

        [native makeKeyAndOrderFront:native];
    }

}

#endif
