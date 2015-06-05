//
// Created by Matthew Hinkle on 5/29/15.
//

#ifndef FORGE_AABB_H
#define FORGE_AABB_H

#include "vec.h"

namespace forge {

    template<typename T>
    struct _aabb {

        _aabb(T i0, T j0, T i1, T j1) : _aabb() { }
        _aabb(_vec2<T> min, _vec2<T> max) : min(min), max(max) { }
        _aabb(const _aabb & a) : min(a.min), max(a.max) { }
        _aabb(_aabb && a) : min(std::move(a.min)), max(std::move(a.max)) { }

        _aabb & operator=(const _aabb & a) {
            this->min = a.min;
            this->max = a.max;
            return *this;
        }

        _aabb & operator=(_aabb && a) {
            this->min = std::move(a.min);
            this->max = std::move(a.max);
            return *this;
        }

        ~_aabb() = default;

        bool intersects(const _aabb & a) const {
            return this->min.x <= a.min.x && this->max.x >= a.max.x
                || this->min.y <= a.min.y && this->max.y >= a.max.y
                || this->min.x <= a.max.x && this->max.x >= a.max.x
                || this->min.y <= a.max.y && this->max.y >= a.max.y;
        }

        _vec2<T> min;
        _vec2<T> max;
    };

    typedef _aabb<float> aabb;
    typedef _aabb<float> aabbf;
    typedef _aabb<double> aabbd;
    typedef _aabb<int> aabbi;
    typedef _aabb<unsigned> aabbu;

}

#endif //FORGE_AABB_H
