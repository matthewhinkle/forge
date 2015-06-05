//
// Created by Matthew Hinkle on 5/29/15.
//

#ifndef FORGE_WINDOW_H
#define FORGE_WINDOW_H

#include <string>

#include "math/rect.h"
#include "math/vec.h"

namespace forge {

    class Window {
    public:
        Window();
        Window(const Window & w);
        Window(Window && w);
        Window & operator=(const Window & w);
        Window & operator=(Window && w);
        ~Window();

        void set_name(const std::string & name);
        void set_bounds(const rect & bounds);
        void set_fullscreen(bool fullscreen);

        const std::string & get_name() const;
        const rect & get_bounds() const;
        bool is_fullscreen() const;

        static std::string default_name() { return "Forge"; }
        static rect default_bounds() { return rect(0, 0, 1024, 768); }

    private:
        std::string name;
        rect bounds;
        bool fullscreen;
    };

}

#endif //FORGE_WINDOW_INCL_H
