cmake_minimum_required(VERSION 3.2)
project(Forge)

# Project options
# ####### #######
option(WITH_XLIB "Use Xlib window implementation" OFF)
option(WITH_XCB "Use Xcb window implementation" OFF)
option(WITH_COCOA "Use Cocoa window implementation" ON)

if(WITH_XLIB)
    add_definitions(-DWITH_XLIB)
elseif(WITH_XCB)
    add_definitions(-DWITH_XCB)
elseif(WITH_COCOA)
    add_definitions(-DWITH_COCOA)
else()
    message(FATAL_ERROR "No window manager selected")
endif()

# Compiler
# ########
if(APPLE AND WITH_COCOA)
    include(CmakeForceCompiler)
    if(IOS)
        cmake_force_c_compiler(clang GNU)
        cmake_force_cxx_compiler(clang++ GNU)
        set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    else()
        cmake_force_c_compiler(llvm-gcc GNU)
        cmake_force_cxx_compiler(llvm-g++ GNU)
        set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvmgcc42")
    endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include path
# ####### ####
include_directories(${PROJECT_SOURCE_DIR}/src)

# Source files
# ###### #####
set(SOURCE_FILES src/main.mm)
add_executable(Forge ${SOURCE_FILES}
                     src/common/logger.cpp
                     src/system/x-window-manager.cpp
                     src/system/xcb-window-manager.cpp
                     src/system/cocoa-window-manager.mm
                     src/AppDelegate.mm
                     src/window-manager.cpp)

if(APPLE)
    set(OPENGL_LIBRARY_DIR /opt/X11/lib/libGL.dylib CACHE FILEPATH "OpenGL library directory path override" FORCE)
    set(XCB_LIBRARY_DIR /opt/X11/lib CACHE FILEPATH "Xcb library directory path override" FORCE)

    if(WITH_XLIB OR WITH_XCB)
        find_package(X11 REQUIRED)
        if(X11_FOUND)
            include_directories(${X11_INCLUDE_DIR})
            target_link_libraries(Forge ${X11_LIBRARIES} ${OPENGL_LIBRARY_DIR})
        endif()

        if(WITH_XCB)
            set(Forge_XCB_LIBRARIES ${XCB_LIBRARY_DIR}/libX11-xcb.dylib
                                    ${XCB_LIBRARY_DIR}/libxcb.dylib
                                    ${XCB_LIBRARY_DIR}/libxcb-keysyms.dylib)

            target_link_libraries(Forge ${Forge_XCB_LIBRARIES})
        endif()
    elseif(WITH_COCOA)
        list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

        find_package(Cocoa REQUIRED)
        include_directories(${Cocoa_INCLUDE_DIRS})
        target_link_libraries(Forge ${Cocoa_LIBRARIES})

        find_package(OpenGL REQUIRED)
        include_directories(${OPENGL_INCLUDE_DIR})
        target_link_libraries(Forge ${OPENGL_LIBRARIES})
    endif()
endif()
