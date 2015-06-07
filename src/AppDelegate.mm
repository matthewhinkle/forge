//
// Created by Matthew Hinkle on 6/6/15.
//

#import "AppDelegate.h"

#include "window-manager.h"

@implementation AppDelegate

- (void) applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSLog(@"hello");

    forge::WindowManager wm;
    forge::Window w = wm.make_window("Forge", forge::rect(200, 200, 800, 600), false);
    forge::OpenGLContext c = wm.make_opengl_context(&w);

    c.make_current();

    NSWindow * window = (__bridge NSWindow *)w.get_window()->native;

    NSModalSession s = [NSApp beginModalSessionForWindow:window];
    while([NSApp runModalSession:s] == NSModalResponseContinue) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.9f, 0.23f, 1);
        c.swap_buffers();
        [window update];
    }
    [NSApp endModalSession:s];
}

@end
