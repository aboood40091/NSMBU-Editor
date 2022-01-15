#ifndef MATH_MTX44_H_
#define MATH_MTX44_H_

#include "math_MTX34.h"
#include "math_VEC4.h"

namespace nw { namespace math {

struct MTX44
{
    MTX44() { }
    MTX44(const MTX44& other) { Copy(this, &other); }

    MTX44(const MTX34& other)
    {
        MTX34::Copy((MTX34*)this, &other);
        m[3][0] = 0.0f;
        m[3][1] = 0.0f;
        m[3][2] = 0.0f;
        m[3][3] = 1.0f;
    }

    MTX44(
        f32 _00, f32 _01, f32 _02, f32 _03,
        f32 _10, f32 _11, f32 _12, f32 _13,
        f32 _20, f32 _21, f32 _22, f32 _23,
        f32 _30, f32 _31, f32 _32, f32 _33
    )
    {
        m[0][0] = _00;
        m[0][1] = _01;
        m[0][2] = _02;
        m[0][3] = _03;

        m[1][0] = _10;
        m[1][1] = _11;
        m[1][2] = _12;
        m[1][3] = _13;

        m[2][0] = _20;
        m[2][1] = _21;
        m[2][2] = _22;
        m[2][3] = _23;

        m[3][0] = _30;
        m[3][1] = _31;
        m[3][2] = _32;
        m[3][3] = _33;
    }

    static const MTX44& Identity()
    {
        static const MTX44 identity(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return identity;
    }

    static MTX44* Copy(MTX44* dst, const MTX44* src)
    {
        if (src != dst)
            *dst = *src;

        return dst;
    }

    static MTX44* Transpose(MTX44* dst, const MTX44* src)
    {
        if (src == dst)
        {
            MTX44 temp;
            temp.m[0][0] = src->m[0][0];
            temp.m[0][1] = src->m[1][0];
            temp.m[0][2] = src->m[2][0];
            temp.m[0][3] = src->m[3][0];
            temp.m[1][0] = src->m[0][1];
            temp.m[1][1] = src->m[1][1];
            temp.m[1][2] = src->m[2][1];
            temp.m[1][3] = src->m[3][1];
            temp.m[2][0] = src->m[0][2];
            temp.m[2][1] = src->m[1][2];
            temp.m[2][2] = src->m[2][2];
            temp.m[2][3] = src->m[3][2];
            temp.m[3][0] = src->m[0][3];
            temp.m[3][1] = src->m[1][3];
            temp.m[3][2] = src->m[2][3];
            temp.m[3][3] = src->m[3][3];
            Copy(dst, &temp);
        }
        else
        {
            dst->m[0][0] = src->m[0][0];
            dst->m[0][1] = src->m[1][0];
            dst->m[0][2] = src->m[2][0];
            dst->m[0][3] = src->m[3][0];
            dst->m[1][0] = src->m[0][1];
            dst->m[1][1] = src->m[1][1];
            dst->m[1][2] = src->m[2][1];
            dst->m[1][3] = src->m[3][1];
            dst->m[2][0] = src->m[0][2];
            dst->m[2][1] = src->m[1][2];
            dst->m[2][2] = src->m[2][2];
            dst->m[2][3] = src->m[3][2];
            dst->m[3][0] = src->m[0][3];
            dst->m[3][1] = src->m[1][3];
            dst->m[3][2] = src->m[2][3];
            dst->m[3][3] = src->m[3][3];
        }
        return dst;
    }

    static MTX44* Concat(MTX44* dst, const MTX44* a, const MTX44* b)
    {
#ifdef EFT_WIN

        const f32 a11 = a->m[0][0];
        const f32 a12 = a->m[0][1];
        const f32 a13 = a->m[0][2];
        const f32 a14 = a->m[0][3];

        const f32 a21 = a->m[1][0];
        const f32 a22 = a->m[1][1];
        const f32 a23 = a->m[1][2];
        const f32 a24 = a->m[1][3];

        const f32 a31 = a->m[2][0];
        const f32 a32 = a->m[2][1];
        const f32 a33 = a->m[2][2];
        const f32 a34 = a->m[2][3];

        const f32 a41 = a->m[3][0];
        const f32 a42 = a->m[3][1];
        const f32 a43 = a->m[3][2];
        const f32 a44 = a->m[3][3];

        const f32 b11 = b->m[0][0];
        const f32 b12 = b->m[0][1];
        const f32 b13 = b->m[0][2];
        const f32 b14 = b->m[0][3];

        const f32 b21 = b->m[1][0];
        const f32 b22 = b->m[1][1];
        const f32 b23 = b->m[1][2];
        const f32 b24 = b->m[1][3];

        const f32 b31 = b->m[2][0];
        const f32 b32 = b->m[2][1];
        const f32 b33 = b->m[2][2];
        const f32 b34 = b->m[2][3];

        const f32 b41 = b->m[3][0];
        const f32 b42 = b->m[3][1];
        const f32 b43 = b->m[3][2];
        const f32 b44 = b->m[3][3];

        dst->m[0][0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
        dst->m[0][1] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
        dst->m[0][2] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
        dst->m[0][3] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

        dst->m[1][0] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
        dst->m[1][1] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
        dst->m[1][2] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
        dst->m[1][3] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

        dst->m[2][0] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
        dst->m[2][1] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
        dst->m[2][2] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
        dst->m[2][3] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

        dst->m[3][0] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
        dst->m[3][1] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
        dst->m[3][2] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
        dst->m[3][3] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

#else

        // Nintendo used their own paired-singles implementation for this, which I will not bother with.
        ASM_MTX44Concat(const_cast<MTX44*>(a)->m, const_cast<MTX44*>(b)->m, dst->m);

#endif // EFT_WIN

        return dst;
    }

    union
    {
        f32  m[4][4];
        VEC4 v[4];
        f32  a[4 * 4];
    };
};

} } // namespace nw::math

#endif // MATH_MTX44_H_
