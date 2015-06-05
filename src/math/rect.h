//
// Created by Matthew Hinkle on 5/29/15.
//

#ifndef FORGE_RECT_H
#define FORGE_RECT_H

namespace forge {

    template<typename T>
    struct _rect {

        _rect(T x, T y, T width, T height) : x(x), y(y), width(width), height(height) { }

        _rect(const _rect & r) :
            x(r.x),
            y(r.y),
            width(r.width),
            height(r.height) { }

        _rect(_rect && r) :
            x(std::move(r.x)),
            y(std::move(r.y)),
            width(std::move(r.width)),
            height(std::move(r.height)) { }

        _rect & operator=(const _rect & r) {
            this->x = r.x;
            this->y = r.y;
            this->width = r.width;
            this->height = r.height;
            return *this;
        }

        _rect & operator=(_rect && r) {
            this->x = std::move(r.x);
            this->y = std::move(r.y);
            this->width = std::move(r.width);
            this->height = std::move(r.height);
            return *this;
        }

        T x;
        T y;
        T width;
        T height;

    };

    typedef _rect<float> rect;
    typedef _rect<float> rectf;
    typedef _rect<double> rectd;
    typedef _rect<int> recti;
    typedef _rect<unsigned> rectu;

}

#endif //FORGE_RECT_H
