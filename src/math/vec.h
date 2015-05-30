//
// Created by Matthew Hinkle on 5/17/15.
//

#ifndef FORGE_VECTOR_H
#define FORGE_VECTOR_H

#include <cmath>
#include <utility>

/*
 * Components:
 *      x, y, z, w
 *      r, g, b, a
 *      s, t, p, q
 */

namespace forge {

    template<typename T>
    struct _vec2 {

        _vec2(const T & t1, const T & t2) : x(t1), y(t2) { }

        _vec2(const _vec2 & v) : x(v.x), y(v.y) { }

        _vec2(_vec2 && v) : x(std::move(v.x)), y(std::move(v.y)) { }

        inline _vec2 & operator=(const _vec2 & v) {
            this->x = v.x;
            this->y = v.y;
            return *this;
        }

        inline _vec2 & operator=(_vec2 && v) {
            this->x = std::move(v.x);
            this->y = std::move(v.y);
            return *this;
        }

        inline _vec2 & operator+=(const _vec2 & v) {
            this->x += v.x;
            this->y += v.y;
            return *this;
        }

        inline _vec2 & operator+=(T t) {
            this->x += t;
            this->y += t;
            return *this;
        }

        inline _vec2 & operator-=(const _vec2 & v) {
            this->x -= v.x;
            this->y -= v.y;
            return *this;
        }

        inline _vec2 & operator-=(T t) {
            this->x -= t;
            this->y -= t;
            return *this;
        }

        inline _vec2 & operator*=(const _vec2 & v) {
            this->x *= v.x;
            this->y *= v.y;
            return *this;
        }

        inline _vec2 & operator*=(T t) {
            this->x *= t;
            this->y *= t;
            return *this;
        }

        inline _vec2 & operator++() {
            this->x++;
            this->y++;
            return *this;
        }

        inline _vec2 operator--() {
            this->x--;
            this->y--;
            return *this;
        }

        inline _vec2 operator++(int) {
            const _vec2 v(*this);
            ++this->x;
            ++this->y;
            return v;
        }

        inline _vec2 operator--(int) {
            const _vec2 v(*this);
            --this->x;
            --this->y;
            return v;
        }

        inline T & operator[](unsigned i) {
            return *(&this->x + i);
        }

        inline T operator[](unsigned i) const {
            return *(&this->x + i);
        }

        inline friend _vec2 operator+(const _vec2 & v, const T & t) {
            return _vec2(v.x + t, v.y + t);
        }

        inline friend _vec2 operator+(const T & t, const _vec2 & v) {
            return _vec2(t + v.x, t + v.y);
        }

        inline friend _vec2 operator+(const _vec2 & v1, const _vec2 & v2) {
            return _vec2(v1.x + v2.x, v1.y + v2.y);
        }

        inline friend _vec2 operator-(const _vec2 & v, const T & t) {
            return _vec2(v.x - t, v.y - t);
        }

        inline friend _vec2 operator-(const T & t, const _vec2 & v) {
            return _vec2(t - v.x, t - v.y);
        }

        inline friend _vec2 operator-(const _vec2 & v1, const _vec2 & v2) {
            return _vec2(v1.x - v2.x, v1.y - v2.y);
        }

        inline friend _vec2 operator-(const _vec2 & v) {
            return _vec2(-v.x, -v.y);
        }

        inline friend _vec2 operator*(const _vec2 & v, const T & t) {
            return _vec2(v.x * t, v.y * t);
        }

        inline friend _vec2 operator*(const T & t, const _vec2 & v) {
            return _vec2(t * v.x, t * v.y);
        }

        inline friend bool operator==(const _vec2 & v1, const _vec2 & v2) {
            return v1.x == v2.x && v1.y == v2.y;
        }

        inline friend bool operator!=(const _vec2 & v1, const _vec2 & v2) {
            return !(v1.x == v2.x && v1.y == v2.y);
        }

        static inline constexpr unsigned size() {
            return 2;
        }

        union {
            T x;
            T r;
            T s;
        };

        union {
            T y;
            T g;
            T t;
        };

    };

    typedef _vec2<float> vec2;
    typedef _vec2<float> vec2f;
    typedef _vec2<double> vec2d;
    typedef _vec2<int> vec2i;
    typedef _vec2<unsigned> vec2u;

    namespace vec {

        template<typename T>
        static inline T dot(const _vec2<T> & v1, const _vec2<T> & v2) {
            return v1.x * v2.x + v1.y * v2.y;
        }

        template<typename T>
        static inline T length(const _vec2<T> & v) {
            return std::sqrt(dot(v, v));
        }

        template<typename T>
        static inline T lengthSquared(const _vec2<T> & v) {
            return dot(v, v);
        }

        template<typename T, typename U>
        static inline T distance(const _vec2<T> & v1, const _vec2<U> & v2) {
            return length(v1 - v2);
        }

        template<typename T>
        static inline _vec2<T> negate(const _vec2<T> & v) {
            return _vec2<T>(0 - v.x, 0 - v.y);
        }

        template<typename T>
        static inline _vec2<T> normalize(const _vec2<T> & v) {
            const T length = vec::length(v);
            if(!(length)) {
                return _vec2<T>(v);
            }

            return v * (1.0f / length);
        }

        template<typename T>
        static inline constexpr unsigned size(const _vec2<T> & v) {
            return 2;
        }

        template<typename T>
        static inline T angle(const _vec2<T> & v1, const _vec2<T> & v2) {
            return std::acos(dot(normalize(v1), normalize(v2)));
        }

    }

    template<typename T>
    struct _vec3 {

        _vec3(const T & t1, const T & t2, const T & t3) : x(t1), y(t2), z(t3) { }

        _vec3(const _vec3 & v) : x(v.x), y(v.y), z(v.z) { }

        _vec3(_vec3 && v) : x(std::move(v.x)), y(std::move(v.y)), z(std::move(v.y)) { }

        inline _vec3 & operator=(const _vec3 & v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            return *this;
        }

        inline _vec3 & operator=(_vec3 && v) {
            this->x = std::move(v.x);
            this->y = std::move(v.y);
            this->z = std::move(v.z);
            return *this;
        }

        inline _vec3 & operator+=(const _vec3 & v) {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
            return *this;
        }

        inline _vec3 & operator+=(const T & k) {
            this->x += k;
            this->y += k;
            this->z += k;
            return *this;
        }

        inline _vec3 & operator-=(const _vec3 & v) {
            this->x -= v.x;
            this->y -= v.y;
            this->z -= v.z;
            return *this;
        }

        inline _vec3 & operator-=(const T & k) {
            this->x -= k;
            this->y -= k;
            this->z -= k;
            return *this;
        }

        inline _vec3 & operator*=(const _vec3 & v) {
            this->x *= v.x;
            this->y *= v.y;
            this->z *= v.z;
            return *this;
        }

        inline _vec3 & operator*=(const T & k) {
            this->x *= k;
            this->y *= k;
            this->z *= k;
            return *this;
        }

        inline _vec3 & operator++() {
            this->x++;
            this->y++;
            this->z++;
            return *this;
        }

        inline _vec3 operator--() {
            this->x--;
            this->y--;
            this->z--;
            return *this;
        }

        inline _vec3 operator++(int) {
            const _vec3 v(*this);
            ++this->x;
            ++this->y;
            ++this->z;
            return v;
        }

        inline _vec3 operator--(int) {
            const _vec3 v(*this);
            --this->x;
            --this->y;
            --this->z;
            return v;
        }

        inline T & operator[](int i) {
            return *(&this->x + i);
        }

        inline T operator[](int i) const {
            return *(&this->x + i);
        }

        inline friend _vec3 operator+(const _vec3 & v, const T & t) {
            return _vec3(v.x + t, v.y + t, v.z + t);
        }

        inline friend _vec3 operator+(const T & t, const _vec3 & v) {
            return _vec3(t + v.x, t + v.y, t + v.z);
        }

        inline friend _vec3 operator+(const _vec3 & v1, const _vec3 & v2) {
            return _vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
        }

        inline friend _vec3 operator-(const _vec3 & v, const T & t) {
            return _vec3(v.x - t, v.y - t, v.z - t);
        }

        inline friend _vec3 operator-(const T & t, const _vec3 & v) {
            return _vec3(t - v.x, t - v.y, t - v.z);
        }

        inline friend _vec3 operator-(const _vec3 & v1, const _vec3 & v2) {
            return _vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
        }

        inline friend _vec3 operator*(const _vec3 & v, const T & t) {
            return _vec3(v.x * t, v.y * t, v.z * t);
        }

        inline friend _vec3 operator*(const T & t, const _vec3 & v) {
            return _vec3(t * v.x, t * v.y, t * v.z);
        }

        inline friend bool operator==(const _vec3 & v1, const _vec3 & v2) {
            const _vec3 diff = v2 - v1;

            return std::abs(diff.x) < 0.0001f
                   && std::abs(diff.y) < 0.0001f
                   && std::abs(diff.z) < 0.0001f;
        }

        inline friend bool operator!=(const _vec3 & v1, const _vec3 & v2) {
            return !(v1 == v2);
        }

        static inline constexpr unsigned size() {
            return 3;
        }

        union {
            T x;
            T r;
            T s;
        };

        union {
            T y;
            T g;
            T t;
        };

        union {
            T z;
            T b;
            T p;
        };
    };

    namespace vec {

        template<typename T>
        static inline _vec3<T> cross(const _vec3<T> & v1, const _vec3<T> & v2) {
            return _vec3<T>(
                    v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.z
            );
        }

        template<typename T>
        static inline T dot(const _vec3<T> & v1, const _vec3<T> & v2) {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }

        template<typename T>
        static inline T length(const _vec3<T> & v) {
            return std::sqrt(dot(v, v));
        }

        template<typename T>
        static inline T lengthSquared(const _vec3<T> & v) {
            return dot(v, v);
        }

        template<typename T>
        static inline T distance(const _vec3<T> & v1, const _vec3<T> & v2) {
            return length(v1 - v2);
        }

        template<typename T>
        static inline _vec3<T> negate(const _vec3<T> & v) {
            return _vec3<T>(0 - v.x, 0 - v.y, 0 - v.z);
        }

        template<typename T>
        static inline _vec3<T> normalize(const _vec3<T> & v) {
            const T length = vec::length(v);
            if(!(length)) {
                return _vec3<T>(v);
            }

            return v * (1.0f / length);
        }

        template<typename T>
        static inline constexpr unsigned size(const _vec3<T> & v) {
            return 3;
        }

        template<typename T>
        static inline T angle(const _vec3<T> & v1, const _vec3<T> & v2) {
            return std::acos(dot(normalize(v1), normalize(v2)));
        }

        template<typename T>
        static inline _vec2<T> xy(const _vec3<T> & v) {
            return _vec2<T>(v.x, v.y);
        }

    }

    template<typename T>
    struct _vec4 {

        _vec4(const T & t = T(0))
                :
                _vec4(t, t, t, t)
        { }

        _vec4(const T & t1, const T & t2, const T & t3, const T & t4)
                :
                x(t1),
                y(t2),
                z(t3),
                w(t4)
        { }

        _vec4(const _vec4 & v)
                :
                x(v.x),
                y(v.y),
                z(v.z),
                w(v.w)
        { }

        inline _vec4 & operator=(const _vec4 & v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = v.w;
            return *this;
        }

        inline _vec4 & operator+=(const _vec4 & v) {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
            this->w += v.w;
            return *this;
        }

        inline _vec4 & operator+=(const T & t) {
            this->x += t;
            this->y += t;
            this->z += t;
            this->w += t;
            return *this;
        }

        inline _vec4 & operator-=(const _vec4 & v) {
            this->x -= v.x;
            this->y -= v.y;
            this->z -= v.z;
            this->w -= v.w;
            return *this;
        }

        inline _vec4 & operator-=(const T & t) {
            this->x -= t;
            this->y -= t;
            this->z -= t;
            this->w -= t;
            return *this;
        }

        inline _vec4 & operator*=(const _vec4 & v) {
            this->x *= v.x;
            this->y *= v.y;
            this->z *= v.z;
            this->w *= v.w;
            return *this;
        }

        inline _vec4 & operator*=(const T & t) {
            this->x *= t;
            this->y *= t;
            this->z *= t;
            this->w *= t;
            return *this;
        }

        inline _vec4 & operator++() {
            this->x++;
            this->y++;
            this->z++;
            this->w++;
            return *this;
        }

        inline _vec4 operator--() {
            this->x--;
            this->y--;
            this->z--;
            this->w--;
            return *this;
        }

        inline _vec4 operator++(int) {
            const _vec4 v(*this);
            ++this->x;
            ++this->y;
            ++this->z;
            ++this->w;
            return v;
        }

        inline _vec4 operator--(int) {
            const _vec4 v(*this);
            --this->x;
            --this->y;
            --this->z;
            --this->w;
            return v;
        }

        inline T & operator[](unsigned i) {
            return *(&this->x + i);
        }

        inline T operator[](unsigned i) const {
            return *(&this->x + i);
        }

        inline friend _vec4 operator+(const _vec4 & v, const T & t) {
            return _vec4(v.x + t, v.y + t, v.z + t, v.w + t);
        }

        inline friend _vec4 operator+(const T & t, const _vec4 & v) {
            return _vec4(t + v.x, t + v.y, t + v.z, t + v.w);
        }

        inline friend _vec4 operator+(const _vec4 & v1, const _vec4 & v2) {
            return _vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
        }

        inline friend _vec4 operator-(const _vec4 & v, const T & t) {
            return _vec4(v.x - t, v.y - t, v.z - t, v.w - t);
        }

        inline friend _vec4 operator-(const T & t, const _vec4 & v) {
            return _vec4(t - v.x, t - v.y, t - v.z, t - v.w);
        }

        inline friend _vec4 operator-(const _vec4 & v1, const _vec4 & v2) {
            return _vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
        }

        inline friend _vec4 operator*(const _vec4 & v, const T & t) {
            return _vec4(v.x * t, v.y * t, v.z * t, v.w * t);
        }

        inline friend _vec4 operator*(const T & t, const _vec4 & v) {
            return _vec4(t * v.x, t * v.y, t * v.z, t * v.w);
        }

        /*
          v * v = transpose(transpose(v) * v
         */
        inline friend _vec4 operator*(const _vec4 & v1, const _vec4 & v2) {
            return _vec4(
                    v1.x*v2.x + v1.x*v2.y + v1.x*v2.z + v1.x*v2.w,
                    v1.y*v2.x + v1.y*v2.y + v1.y*v2.z + v1.y*v2.w,
                    v1.z*v2.x + v1.z*v2.y + v1.z*v2.z + v1.z*v2.w,
                    v1.w*v2.x + v1.w*v2.y + v1.w*v2.z + v1.w*v2.w
            );
        }

        inline friend _vec4 operator*(const _vec3<T> & v1, const _vec4 & v2) {
            return _vec4(
                    v1.x*v2.x + v1.x*v2.y + v1.x*v2.z + v1.x*v2.w,
                    v1.y*v2.x + v1.y*v2.y + v1.y*v2.z + v1.y*v2.w,
                    v1.z*v2.x + v1.z*v2.y + v1.z*v2.z + v1.z*v2.w,
                    v2.x + v2.y + v2.z + v2.w
            );
        }

        inline friend _vec3<T> operator*(const _vec4 & v1, const _vec3<T> & v2) {
            return _vec3<T>(
                    v1.x*v2.x + v1.x*v2.y + v1.x*v2.z + v1.x,
                    v1.y*v2.x + v1.y*v2.y + v1.y*v2.z + v1.z,
                    v1.z*v2.x + v1.z*v2.y + v1.z*v2.z + v1.z
            );
        }

        inline friend bool operator==(const _vec4 & v1, const _vec4 & v2) {
            return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
        }

        inline friend bool operator!=(const _vec4 & v1, const _vec4 & v2) {
            return !(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
        }

        static inline constexpr unsigned size() {
            return 4;
        }

        union {
            T x;
            T r;
            T s;
        };

        union {
            T y;
            T g;
            T t;
        };

        union {
            T z;
            T b;
            T p;
        };

        union {
            T w;
            T a;
            T q;
        };
    };

    namespace vec {

        template<typename T>
        static inline _vec3<T> cross(const _vec4<T> & v1, const _vec4<T> & v2) {
            return _vec3<T>(
                    v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.z
            );
        }

        template<typename T>
        static inline _vec4<T> cross(const _vec4<T> & v1, const _vec4<T> & v2) {
            return _vec4<T>(
                    v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.z,
                    T(0)
            );
        }

        template<typename T>
        static inline T dot(const _vec4<T> & v1, const _vec4<T> & v2) {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
        }

        template<typename T>
        static inline T length(const _vec4<T> & v) {
            return std::sqrt(dot(v, v));
        }

        template<typename T>
        static inline T lengthSquared(const _vec4<T> & v) {
            return dot(v, v);
        }

        template<typename T>
        static inline T distance(const _vec4<T> & v1, const _vec4<T> & v2) {
            return length(v1 - v2);
        }

        template<typename T>
        static inline _vec4<T> negate(const _vec4<T> & v) {
            return _vec4<T>(0 - v.x, 0 - v.y, 0 - v.z, 0 - v.w);
        }

        template<typename T>
        static inline _vec4<T> normalize(const _vec4<T> & v) {
            const T length = vec::length(v);
            if(!(length)) {
                return _vec4<T>(v);
            }

            return v * (1.0f / length);
        }

        template<typename T>
        static inline constexpr unsigned size(const _vec4<T> & v) {
            return 4;
        }

        template<typename T>
        static inline T angle(const _vec4<T> & v1, const _vec4<T> & v2) {
            return std::acos(dot(normalize(v1), normalize(v2)));
        }

        template<typename T>
        static inline _vec2<T> xy(const _vec4<T> & v) {
            return _vec2<T>(v.x, v.y);
        }

        template<typename T>
        static inline _vec3<T> xyz(const _vec4<T> & v) {
            return _vec3<T>(v.x, v.y, v.z);
        }

    }

}

#endif //FORGE_VECTOR_H
