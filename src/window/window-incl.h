//
// Created by Matthew Hinkle on 5/29/15.
//

#ifndef FORGE_WINDOW_INCL_H
#define FORGE_WINDOW_INCL_H

#include <string>

namespace forge {

    class Window {
    public:
        Window() { }
        Window(const Window & w);
        Window(Window && w);
        Window & operator=(const Window & w);
        Window & operator=(Window && w);
        ~Window() { }

        void set_name(const std::string & name);
        void set_position(const vec2i & position);
        void set_size(const vec2i & size);

        const std::string & get_name() const;
        const vec2i & get_position() const;
        const vec2i & get_size() const;
        bool is_fullscreen() const;

        void display_fullscreen();
        void display_windowed();

        inline static constexpr std::string default_name() { return "Forge"; }
        inline static constexpr vec2i default_position() { return vec2i(); }
        inline static constexpr vec2i default_size() { return vec2i(1024, 768); }

    private:
        std::string name;
        vec2i position;
        vec2i size;
        bool is_fullscreen;

        struct window * w;
    };

}

#endif //FORGE_WINDOW_INCL_H
