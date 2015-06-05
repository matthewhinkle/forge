//
// Created by Matthew Hinkle on 5/30/15.
//

#ifndef FORGE_GL_H
#define FORGE_GL_H

#if defined (__APPLE__) && defined (__MACH__)
    #ifdef WITH_XLIB
        #include <GL/gl.h>
        #include <GL/glx.h>
    #else
        #if !defined (GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED)
            #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
        #endif
        #include <OpenGL/gl3.h>
#endif
#elif defined (__unix__) || defined (__linux__)
    #include <GL/gl.h>
    #include <GL/glx.h>
#elif defined (_WIN32) || defined (_WIN64)
    #include <gl/gl.h>
#else
    #error "OpenGL platform target not supported"
#endif

#endif //FORGE_GL_H
