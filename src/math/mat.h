//
// Created by Matthew Hinkle on 5/17/15.
//

#ifndef FORGE_MATRIX_H
#define FORGE_MATRIX_H

#include <cmath>
#include <cstdlib>
#include <cstring>

#include "vec.h"

#define m00 values[0]
#define m01 values[4]
#define m02 values[8]
#define m03 values[12]

#define m10 values[1]
#define m11 values[5]
#define m12 values[9]
#define m13 values[13]

#define m20 values[2]
#define m21 values[6]
#define m22 values[10]
#define m23 values[14]

#define m30 values[3]
#define m31 values[7]
#define m32 values[11]
#define m33 values[15]

namespace forge {

    template<typename T>
    struct _mat4 {

        _mat4() {
            memset(this->values, 0, sizeof(this->values));
            m00 = m11 = m22 = m33 = T(1);
        }

        _mat4(
                const T & a00, const T & a01, const T & a02, const T & a03,
                const T & a10, const T & a11, const T & a12, const T & a13,
                const T & a20, const T & a21, const T & a22, const T & a23,
                const T & a30, const T & a31, const T & a32, const T & a33
        ) {
            this->m00 = a00;
            this->m01 = a01;
            this->m02 = a02;
            this->m03 = a03;

            this->m10 = a10;
            this->m11 = a11;
            this->m12 = a12;
            this->m13 = a13;

            this->m20 = a20;
            this->m21 = a21;
            this->m22 = a22;
            this->m23 = a23;

            this->m30 = a30;
            this->m31 = a31;
            this->m32 = a32;
            this->m33 = a33;
        }

        _mat4(const _mat4 & m) {
            memcpy(this->values, m.values, sizeof(this->values));
        }

        _mat4(_mat4 && m) : values(std::move(m.values)) { }

        virtual ~_mat4() { }

        inline _mat4 & operator=(const _mat4 & m) {
            memcpy(this->values, m.values, sizeof(this->values));
            return *this;
        }

        inline _mat4 & operator+=(const _mat4 & m) {
            m00 += m.m00;
            m01 += m.m01;
            m02 += m.m02;
            m03 += m.m03;

            m10 += m.m10;
            m11 += m.m11;
            m12 += m.m12;
            m13 += m.m13;

            m20 += m.m20;
            m21 += m.m21;
            m22 += m.m22;
            m23 += m.m23;

            m30 += m.m30;
            m31 += m.m31;
            m32 += m.m32;
            m33 += m.m33;

            return *this;
        }

        inline _mat4 & operator+=(const T & t) {
            m00 += t;
            m01 += t;
            m02 += t;
            m03 += t;

            m10 += t;
            m11 += t;
            m12 += t;
            m13 += t;

            m20 += t;
            m21 += t;
            m22 += t;
            m23 += t;

            m30 += t;
            m31 += t;
            m32 += t;
            m33 += t;

            return *this;
        }

        inline _mat4 & operator-=(const _mat4 & m) {
            m00 -= m.m00;
            m01 -= m.m01;
            m02 -= m.m02;
            m03 -= m.m03;

            m10 -= m.m10;
            m11 -= m.m11;
            m12 -= m.m12;
            m13 -= m.m13;

            m20 -= m.m20;
            m21 -= m.m21;
            m22 -= m.m22;
            m23 -= m.m23;

            m30 -= m.m30;
            m31 -= m.m31;
            m32 -= m.m32;
            m33 -= m.m33;

            return *this;
        }

        inline _mat4 & operator-=(const T & t) {
            m00 -= t;
            m01 -= t;
            m02 -= t;
            m03 -= t;

            m10 -= t;
            m11 -= t;
            m12 -= t;
            m13 -= t;

            m20 -= t;
            m21 -= t;
            m22 -= t;
            m23 -= t;

            m30 -= t;
            m31 -= t;
            m32 -= t;
            m33 -= t;

            return *this;
        }

        inline _mat4 & operator*=(const _mat4 & m) {
            _mat4 a;

            a.m00 = m00*m.m00 + m01*m.m10 + m02*m.m20 + m03*m.m30;
            a.m10 = m10*m.m00 + m11*m.m10 + m12*m.m20 + m13*m.m30;
            a.m20 = m20*m.m00 + m21*m.m10 + m22*m.m20 + m23*m.m30;
            a.m30 = m30*m.m00 + m31*m.m10 + m32*m.m20 + m33*m.m30;

            a.m01 = m00*m.m01 + m01*m.m11 + m02*m.m21 + m03*m.m31;
            a.m11 = m10*m.m01 + m11*m.m11 + m12*m.m21 + m13*m.m31;
            a.m21 = m20*m.m01 + m21*m.m11 + m22*m.m21 + m23*m.m31;
            a.m31 = m30*m.m01 + m31*m.m11 + m32*m.m21 + m33*m.m31;

            a.m02 = m00*m.m02 + m01*m.m12 + m02*m.m22 + m03*m.m32;
            a.m12 = m10*m.m02 + m11*m.m12 + m12*m.m22 + m13*m.m32;
            a.m22 = m20*m.m02 + m21*m.m12 + m22*m.m22 + m23*m.m32;
            a.m32 = m30*m.m02 + m31*m.m12 + m32*m.m22 + m33*m.m32;

            a.m03 = m00*m.m03 + m01*m.m13 + m02*m.m23 + m03*m.m33;
            a.m13 = m10*m.m03 + m11*m.m13 + m12*m.m23 + m13*m.m33;
            a.m23 = m20*m.m03 + m21*m.m13 + m22*m.m23 + m23*m.m33;
            a.m33 = m30*m.m03 + m31*m.m13 + m32*m.m23 + m33*m.m33;

            return (*this = a);
        }

        inline _mat4 & operator*=(const T & t) {
            m00 *= t;
            m01 *= t;
            m02 *= t;
            m03 *= t;

            m10 *= t;
            m11 *= t;
            m12 *= t;
            m13 *= t;

            m20 *= t;
            m21 *= t;
            m22 *= t;
            m23 *= t;

            m30 *= t;
            m31 *= t;
            m32 *= t;
            m33 *= t;

            return *this;
        }

        inline _mat4 & operator++() {
            m00++;
            m01++;
            m02++;
            m03++;

            m10++;
            m11++;
            m12++;
            m13++;

            m20++;
            m21++;
            m22++;
            m23++;

            m30++;
            m31++;
            m32++;
            m33++;

            return *this;
        }

        inline _mat4 & operator--() {
            m00--;
            m01--;
            m02--;
            m03--;

            m10--;
            m11--;
            m12--;
            m13--;

            m20--;
            m21--;
            m22--;
            m23--;

            m30--;
            m31--;
            m32--;
            m33--;

            return *this;
        }

        inline _mat4 operator++(int) {
            const _mat4 m(*this);

            ++m00;
            ++m01;
            ++m02;
            ++m03;

            ++m10;
            ++m11;
            ++m12;
            ++m13;

            ++m20;
            ++m21;
            ++m22;
            ++m23;

            ++m30;
            ++m31;
            ++m32;
            ++m33;

            return m;
        }

        inline _mat4 operator--(int) {
            const _mat4 m(*this);

            --m00;
            --m01;
            --m02;
            --m03;

            --m10;
            --m11;
            --m12;
            --m13;

            --m20;
            --m21;
            --m22;
            --m23;

            --m30;
            --m31;
            --m32;
            --m33;

            return m;
        }

        inline T operator()(int i, int j) const {
            return this->values[(i * 4) + j];
        }

        inline T & operator()(int i, int j) {
            return this->values[(i * 4) + j];
        }

        inline friend _mat4 operator+(const _mat4 & m1, const _mat4 & m2) {
            _mat4 a;

            a.m00 = m1.m00 + m2.m00;
            a.m01 = m1.m01 + m2.m01;
            a.m02 = m1.m02 + m2.m02;
            a.m03 = m1.m03 + m2.m03;

            a.m10 = m1.m10 + m2.m10;
            a.m11 = m1.m11 + m2.m11;
            a.m12 = m1.m12 + m2.m12;
            a.m13 = m1.m13 + m2.m13;

            a.m20 = m1.m20 + m2.m20;
            a.m21 = m1.m21 + m2.m21;
            a.m22 = m1.m22 + m2.m22;
            a.m23 = m1.m23 + m2.m23;

            a.m30 = m1.m30 + m2.m30;
            a.m31 = m1.m31 + m2.m31;
            a.m32 = m1.m32 + m2.m32;
            a.m33 = m1.m33 + m2.m33;

            return a;
        }

        inline friend _mat4 operator+(const _mat4 & m, const T & t) {
            _mat4 a;

            a.m00 = m.m00 + t;
            a.m01 = m.m01 + t;
            a.m02 = m.m02 + t;
            a.m03 = m.m03 + t;

            a.m10 = m.m10 + t;
            a.m11 = m.m11 + t;
            a.m12 = m.m12 + t;
            a.m13 = m.m13 + t;

            a.m20 = m.m20 + t;
            a.m21 = m.m21 + t;
            a.m22 = m.m22 + t;
            a.m23 = m.m23 + t;

            a.m30 = m.m30 + t;
            a.m31 = m.m31 + t;
            a.m32 = m.m32 + t;
            a.m33 = m.m33 + t;

            return a;
        }

        inline friend _mat4 operator+(const T & t, const _mat4 & m) {
            _mat4 a;

            a.m00 = t + m.m00;
            a.m01 = t + m.m01;
            a.m02 = t + m.m02;
            a.m03 = t + m.m03;

            a.m10 = t + m.m10;
            a.m11 = t + m.m11;
            a.m12 = t + m.m12;
            a.m13 = t + m.m13;

            a.m20 = t + m.m20;
            a.m21 = t + m.m21;
            a.m22 = t + m.m22;
            a.m23 = t + m.m23;

            a.m30 = t + m.m30;
            a.m31 = t + m.m31;
            a.m32 = t + m.m32;
            a.m33 = t + m.m33;

            return a;
        }

        inline friend _mat4 operator-(const _mat4 & m1, const _mat4 & m2) {
            _mat4 a;

            a.m00 = m1.m00 - m2.m00;
            a.m01 = m1.m01 - m2.m01;
            a.m02 = m1.m02 - m2.m02;
            a.m03 = m1.m03 - m2.m03;

            a.m10 = m1.m10 - m2.m10;
            a.m11 = m1.m11 - m2.m11;
            a.m12 = m1.m12 - m2.m12;
            a.m13 = m1.m13 - m2.m13;

            a.m20 = m1.m20 - m2.m20;
            a.m21 = m1.m21 - m2.m21;
            a.m22 = m1.m22 - m2.m22;
            a.m23 = m1.m23 - m2.m23;

            a.m30 = m1.m30 - m2.m30;
            a.m31 = m1.m31 - m2.m31;
            a.m32 = m1.m32 - m2.m32;
            a.m33 = m1.m33 - m2.m33;

            return a;
        }

        inline friend _mat4 operator-(const _mat4 & m, const T & t) {
            _mat4 a;

            a.m00 = m.m00 - t;
            a.m01 = m.m01 - t;
            a.m02 = m.m02 - t;
            a.m03 = m.m03 - t;

            a.m10 = m.m10 - t;
            a.m11 = m.m11 - t;
            a.m12 = m.m12 - t;
            a.m13 = m.m13 - t;

            a.m20 = m.m20 - t;
            a.m21 = m.m21 - t;
            a.m22 = m.m22 - t;
            a.m23 = m.m23 - t;

            a.m30 = m.m30 - t;
            a.m31 = m.m31 - t;
            a.m32 = m.m32 - t;
            a.m33 = m.m33 - t;

            return a;
        }

        inline friend _mat4 operator-(const T & t, const _mat4 & m) {
            _mat4 a;

            a.m00 = t - m.m00;
            a.m01 = t - m.m01;
            a.m02 = t - m.m02;
            a.m03 = t - m.m03;

            a.m10 = t - m.m10;
            a.m11 = t - m.m11;
            a.m12 = t - m.m12;
            a.m13 = t - m.m13;

            a.m20 = t - m.m20;
            a.m21 = t - m.m21;
            a.m22 = t - m.m22;
            a.m23 = t - m.m23;

            a.m30 = t - m.m30;
            a.m31 = t - m.m31;
            a.m32 = t - m.m32;
            a.m33 = t - m.m33;

            return a;
        }

        inline friend _mat4 operator*(const _mat4 & m1, const _mat4 & m2) {
            _mat4 a;

            a.m00 = m1.m00*m2.m00 + m1.m01*m2.m10 + m1.m02*m2.m20 + m1.m03*m2.m30;
            a.m10 = m1.m10*m2.m00 + m1.m11*m2.m10 + m1.m12*m2.m20 + m1.m13*m2.m30;
            a.m20 = m1.m20*m2.m00 + m1.m21*m2.m10 + m1.m22*m2.m20 + m1.m23*m2.m30;
            a.m30 = m1.m30*m2.m00 + m1.m31*m2.m10 + m1.m32*m2.m20 + m1.m33*m2.m30;

            a.m01 = m1.m00*m2.m01 + m1.m01*m2.m11 + m1.m02*m2.m21 + m1.m03*m2.m31;
            a.m11 = m1.m10*m2.m01 + m1.m11*m2.m11 + m1.m12*m2.m21 + m1.m13*m2.m31;
            a.m21 = m1.m20*m2.m01 + m1.m21*m2.m11 + m1.m22*m2.m21 + m1.m23*m2.m31;
            a.m31 = m1.m30*m2.m01 + m1.m31*m2.m11 + m1.m32*m2.m21 + m1.m33*m2.m31;

            a.m02 = m1.m00*m2.m02 + m1.m01*m2.m12 + m1.m02*m2.m22 + m1.m03*m2.m32;
            a.m12 = m1.m10*m2.m02 + m1.m11*m2.m12 + m1.m12*m2.m22 + m1.m13*m2.m32;
            a.m22 = m1.m20*m2.m02 + m1.m21*m2.m12 + m1.m22*m2.m22 + m1.m23*m2.m32;
            a.m32 = m1.m30*m2.m02 + m1.m31*m2.m12 + m1.m32*m2.m22 + m1.m33*m2.m32;

            a.m03 = m1.m00*m2.m03 + m1.m01*m2.m13 + m1.m02*m2.m23 + m1.m03*m2.m33;
            a.m13 = m1.m10*m2.m03 + m1.m11*m2.m13 + m1.m12*m2.m23 + m1.m13*m2.m33;
            a.m23 = m1.m20*m2.m03 + m1.m21*m2.m13 + m1.m22*m2.m23 + m1.m23*m2.m33;
            a.m33 = m1.m30*m2.m03 + m1.m31*m2.m13 + m1.m32*m2.m23 + m1.m33*m2.m33;

            return a;
        }

        inline friend _mat4 operator*(const _mat4 & m, const T & t) {
            _mat4 a;

            a.m00 = m.m00 * t;
            a.m01 = m.m01 * t;
            a.m02 = m.m02 * t;
            a.m03 = m.m03 * t;

            a.m10 = m.m10 * t;
            a.m11 = m.m11 * t;
            a.m12 = m.m12 * t;
            a.m13 = m.m13 * t;

            a.m20 = m.m20 * t;
            a.m21 = m.m21 * t;
            a.m22 = m.m22 * t;
            a.m23 = m.m23 * t;

            a.m30 = m.m30 * t;
            a.m31 = m.m31 * t;
            a.m32 = m.m32 * t;
            a.m33 = m.m33 * t;

            return a;
        }

        inline friend _mat4 operator*(const T & t, const _mat4 & m) {
            _mat4 a;

            a.m00 = t * m.m00;
            a.m01 = t * m.m01;
            a.m02 = t * m.m02;
            a.m03 = t * m.m03;

            a.m10 = t * m.m10;
            a.m11 = t * m.m11;
            a.m12 = t * m.m12;
            a.m13 = t * m.m13;

            a.m20 = t * m.m20;
            a.m21 = t * m.m21;
            a.m22 = t * m.m22;
            a.m23 = t * m.m23;

            a.m30 = t * m.m30;
            a.m31 = t * m.m31;
            a.m32 = t * m.m32;
            a.m33 = t * m.m33;

            return a;
        }

        inline friend _vec3<T> operator*(const _mat4 & m, const _vec3<T> & v) {
            return _vec3<T>(
                    m.m00*v.x + m.m01*v.y + m.m02*v.z + m.m03,
                    m.m10*v.x + m.m11*v.y + m.m12*v.z + m.m13,
                    m.m20*v.x + m.m21*v.y + m.m22*v.z + m.m23
            );
        }

        /**
          v * m = transpose(transpose(v) * m)
         */
        inline friend _vec3<T> operator*(const _vec3<T> & v, const _mat4 & m) {
            return _vec3<T>(
                    v.x*m.m00 + v.y*m.m10 + v.z*m.m20 + m.m30,
                    v.x*m.m01 + v.y*m.m11 + v.z*m.m21 + m.m31,
                    v.x*m.m02 + v.y*m.m12 + v.z*m.m22 + m.m32
            );
        }

        inline friend _vec4<T> operator*(const _mat4 & m, const _vec4<T> & v) {
            return _vec4<T>(
                    m.m00*v.x + m.m01*v.y + m.m02*v.z + m.m03*v.w,
                    m.m10*v.x + m.m11*v.y + m.m12*v.z + m.m13*v.w,
                    m.m20*v.x + m.m21*v.y + m.m22*v.z + m.m23*v.w,
                    m.m30*v.x + m.m31*v.y + m.m32*v.z + m.m33*v.w
            );
        }

        /**
          v * m = transpose(transpose(v) * m)
         */
        inline friend _vec4<T> operator*(const _vec4<T> & v, const _mat4 & m) {
            return _vec4<T>(
                    v.x*m.m00 + v.y*m.m10 + v.z*m.m20 + v.w*m.m30,
                    v.x*m.m01 + v.y*m.m11 + v.z*m.m21 + v.w*m.m31,
                    v.x*m.m02 + v.y*m.m12 + v.z*m.m22 + v.w*m.m32,
                    v.x*m.m03 + v.y*m.m13 + v.z*m.m23 + v.w*m.m33
            );
        }

        inline friend bool operator==(const _mat4 & m1, const _mat4 & m2) {
            return m1.m00 == m2.m00
                   && m1.m01 == m2.m01
                   && m1.m02 == m2.m02
                   && m1.m03 == m2.m03

                   && m1.m10 == m2.m10
                   && m1.m11 == m2.m11
                   && m1.m12 == m2.m12
                   && m1.m13 == m2.m13

                   && m1.m20 == m2.m20
                   && m1.m21 == m2.m21
                   && m1.m22 == m2.m22
                   && m1.m23 == m2.m23

                   && m1.m30 == m2.m30
                   && m1.m31 == m2.m31
                   && m1.m32 == m2.m32
                   && m1.m33 == m2.m33;
        }

        inline friend bool operator!=(const _mat4 & m1, const _mat4 & m2) {
            return !(m1 == m2);
        }

        T values[16];
    };

    namespace mat {

        template<typename T>
        static inline _mat4<T> transpose(const _mat4<T> & m) {
            _mat4<T> a;

            a.m00 = m.m00;
            a.m01 = m.m10;
            a.m02 = m.m20;
            a.m03 = m.m30;

            a.m10 = m.m01;
            a.m11 = m.m11;
            a.m12 = m.m21;
            a.m13 = m.m31;

            a.m20 = m.m02;
            a.m21 = m.m12;
            a.m22 = m.m22;
            a.m23 = m.m32;

            a.m30 = m.m03;
            a.m31 = m.m13;
            a.m32 = m.m23;
            a.m33 = m.m33;

            return a;
        }

    }

    typedef _mat4<float> mat4;
    typedef _mat4<float> mat4f;
    typedef _mat4<double> mat4d;
    typedef _mat4<int> mat4i;
    typedef _mat4<unsigned> mat4u;
}

#undef m00
#undef m01
#undef m02
#undef m03

#undef m10
#undef m11
#undef m12
#undef m13

#undef m20
#undef m21
#undef m22
#undef m23

#undef m30
#undef m31
#undef m32
#undef m33

#endif //FORGE_MATRIX_H
