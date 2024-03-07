// Copyright (c) 2024 Wildan R Wijanarko (@wildan9)

// This software is provided "as-is", without any express or implied warranty. In no event 
// will the authors be held liable for any damages arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose, including commercial 
// applications, and to alter it and redistribute it freely, subject to the following restrictions:

//   1. The origin of this software must not be misrepresented; you must not claim that you 
//   wrote the original software. If you use this software in a product, an acknowledgment 
//   in the product documentation would be appreciated but is not required.

//   2. Altered source versions must be plainly marked as such, and must not be misrepresented
//   as being the original software.

//   3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "raymath.h"

struct Transform3D
{
    Vector3 position{ 0.0f, 0.0f, 0.0f };
    Quaternion rotation{ 0.0f, 0.0f, 0.0f, 1.0f };
    Vector3 scale{ 1.0f, 1.0f, 1.0f };
    Matrix ToMatrix() const;
};

inline Vector3 operator +(const Vector3& a, const Vector3& b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vector3 operator -(const Vector3& a, const Vector3& b)
{
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vector3 operator *(const Vector3& a, const Vector3& b)
{
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

inline Matrix operator *(const Matrix& a, const Matrix& b)
{
    return {
        a.m0 * b.m0 + a.m4 * b.m1 + a.m8 * b.m2 + a.m12 * b.m3,
        a.m1 * b.m0 + a.m5 * b.m1 + a.m9 * b.m2 + a.m13 * b.m3,
        a.m2 * b.m0 + a.m6 * b.m1 + a.m10 * b.m2 + a.m14 * b.m3,
        a.m3 * b.m0 + a.m7 * b.m1 + a.m11 * b.m2 + a.m15 * b.m3,

        a.m0 * b.m4 + a.m4 * b.m5 + a.m8 * b.m6 + a.m12 * b.m7,
        a.m1 * b.m4 + a.m5 * b.m5 + a.m9 * b.m6 + a.m13 * b.m7,
        a.m2 * b.m4 + a.m6 * b.m5 + a.m10 * b.m6 + a.m14 * b.m7,
        a.m3 * b.m4 + a.m7 * b.m5 + a.m11 * b.m6 + a.m15 * b.m7,

        a.m0 * b.m8 + a.m4 * b.m9 + a.m8 * b.m10 + a.m12 * b.m11,
        a.m1 * b.m8 + a.m5 * b.m9 + a.m9 * b.m10 + a.m13 * b.m11,
        a.m2 * b.m8 + a.m6 * b.m9 + a.m10 * b.m10 + a.m14 * b.m11,
        a.m3 * b.m8 + a.m7 * b.m9 + a.m11 * b.m10 + a.m15 * b.m11,

        a.m0 * b.m12 + a.m4 * b.m13 + a.m8 * b.m14 + a.m12 * b.m15,
        a.m1 * b.m12 + a.m5 * b.m13 + a.m9 * b.m14 + a.m13 * b.m15,
        a.m2 * b.m12 + a.m6 * b.m13 + a.m10 * b.m14 + a.m14 * b.m15,
        a.m3 * b.m12 + a.m7 * b.m13 + a.m11 * b.m14 + a.m15 * b.m15
    };
}

inline Matrix MatrixTranslateV(const Vector3& v)
{
    return { 
        1.0f, 0.0f, 0.0f, v.x,
        0.0f, 1.0f, 0.0f, v.y,
        0.0f, 0.0f, 1.0f, v.z,
        0.0f, 0.0f, 0.0f, 1.0f 
    };
}

inline Matrix MatrixScaleV(const Vector3& v)
{
    return {
        v.x, 0.0f, 0.0f, 0.0f,
        0.0f, v.y, 0.0f, 0.0f,
        0.0f, 0.0f, v.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f 
    };
}

inline Matrix Transform3D::ToMatrix() const
{
    return MatrixTranslateV(position) * QuaternionToMatrix(rotation) * MatrixScaleV(scale);
}