cmake_minimum_required(VERSION 3.2)
project(Forge)

# Compiler flags
# ######## #####
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include path
# ####### ####
include_directories(${PROJECT_SOURCE_DIR}/src)

# Source files
# ###### #####
set(SOURCE_FILES src/main.cpp)
add_executable(Forge ${SOURCE_FILES}
                     src/common/logger.cpp
                     src/window.cpp
                     src/x-window.cpp
                     src/system/x-window-manager.cpp)

# Project options
# ####### #######
option(WITH_XLIB "Use Xlib window implementation" ON)
option(WITH_XCB "Use Xcb window implementation" OFF)
option(WITH_COCOA "Use Cocoa window implementation" OFF)

if(WITH_XLIB)
    add_definitions(-DWITH_XLIB)
elseif(WITH_XCB)
    add_definitions(-DWITH_XCB)
elseif(WITH_COCOA)
    add_definitions(-DWITH_COCOA)
else()
    message(FATAL_ERROR "No window manager selected")
endif(WITH_XLIB)

if(APPLE)
    set(Forge_OPENGL_LIBRARY_PATH /opt/X11/lib/libGL.dylib)

    if(WITH_XLIB)
        find_package(X11 REQUIRED)
        if(X11_FOUND)
            include_directories(${X11_INCLUDE_DIR})
            set(EXTRA_LIBS ${Forge_OPENGL_LIBRARY_PATH})
            target_link_libraries(Forge ${X11_LIBRARIES} ${EXTRA_LIBS})
        endif(X11_FOUND)
    else()
        # TODO include Cocoa
        find_package(OpenGL REQUIRED)
        if(OPENGL_FOUND)
            include_directories(${OPENGL_INCLUDE_DIR})
            target_link_libraries(Forge ${OPENGL_LIBRARIES})
        endif(OPENGL_FOUND)
    endif(WITH_XLIB)
endif(APPLE)
