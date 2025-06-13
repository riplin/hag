//Copyright 2023-Present riplin

#pragma once

#include <hag/math/fp/v4.h>

namespace Hag { namespace Math {

class m44
{
public:
    inline m44() {}
    explicit inline m44(v4 r0, v4 r1, v4 r2, v4 r3)
        : m_r0(r0), m_r1(r1), m_r2(r2), m_r3(r3) {}

    static m44 Identity;
    static v4 X;
    static v4 Y;
    static v4 Z;
    static v4 W;

    inline const m44& operator=(const m44& rhs)
    { m_r0 = rhs.m_r0; m_r1 = rhs.m_r1; m_r2 = rhs.m_r2; m_r3 = rhs.m_r3; return (*this); }

    inline const v4& r0() const { return m_r0; }
    inline v4& r0() { return m_r0; }
    
    inline const v4& r1() const { return m_r1; }
    inline v4& r1() { return m_r1; }

    inline const v4& r2() const { return m_r2; }
    inline v4& r2() { return m_r2; }

    inline const v4& r3() const { return m_r3; }
    inline v4& r3() { return m_r3; }

    inline v4 c0() const { return v4(m_r0.x(), m_r1.x(), m_r2.x(), m_r3.x()); }
    inline v4 c1() const { return v4(m_r0.y(), m_r1.y(), m_r2.y(), m_r3.y()); }
    inline v4 c2() const { return v4(m_r0.z(), m_r1.z(), m_r2.z(), m_r3.z()); }
    inline v4 c3() const { return v4(m_r0.w(), m_r1.w(), m_r2.w(), m_r3.w()); }
    
    inline m44 operator*(const m44& rhs)
    {
        v4 rhsc0 = rhs.c0(); v4 rhsc1 = rhs.c1();
        v4 rhsc2 = rhs.c2(); v4 rhsc3 = rhs.c3();
        return m44(v4(m_r0.Dot(rhsc0), m_r0.Dot(rhsc1), m_r0.Dot(rhsc2), m_r0.Dot(rhsc3)),
                   v4(m_r1.Dot(rhsc0), m_r1.Dot(rhsc1), m_r1.Dot(rhsc2), m_r1.Dot(rhsc3)),
                   v4(m_r2.Dot(rhsc0), m_r2.Dot(rhsc1), m_r2.Dot(rhsc2), m_r2.Dot(rhsc3)),
                   v4(m_r3.Dot(rhsc0), m_r3.Dot(rhsc1), m_r3.Dot(rhsc2), m_r3.Dot(rhsc3)));
    }

    inline v4 operator*(const v4& rhs)
    { return v4(m_r0.Dot(rhs), m_r1.Dot(rhs), m_r2.Dot(rhs), m_r3.Dot(rhs)); }

    inline static m44 RotateX(fp angle)
    { 
        fp s = angle.Sin(); fp c = angle.Cos();
        return m44(X,
                   v4(  0,  c, s.Neg(),  0),
                   v4(  0,  s,       c,  0),
                   W);
    }

    inline static m44 RotateY(fp angle)
    {
        fp s = angle.Sin(); fp c = angle.Cos();
        return m44(v4(       c,  0,  s,  0),
                   Y,
                   v4( s.Neg(),  0,  c,  0),
                   W);
    }
    
    inline static m44 RotateZ(fp angle)
    {
        fp s = angle.Sin(); fp c = angle.Cos();
        return m44(v4(  c, s.Neg(),  0,  0),
                   v4(  s,       c,  0,  0),
                   Z,
                   W);
    }
    
    inline static m44 Translate(const v3& pos)
    {
        return m44(v4(1, 0, 0, pos.x()),
                   v4(0, 1, 0, pos.y()),
                   v4(0, 0, 1, pos.z()),
                   W);
    }

    inline static m44 Translate(const v4& pos)
    { return Translate(pos.xyz()); }

    inline static m44 Scale(const v3& scale)
    {
        return m44(v4(scale.x(),         0,         0, 0),
                   v4(        0, scale.y(),         0, 0),
                   v4(        0,         0, scale.z(), 0),
                   W);
    }

    inline static m44 Scale(const v4& scale)
    { return Scale(scale.xyz()); }

    inline static m44 LookAt(const v3& up, const v3& from, const v3& at)
    {
        v3 forward = (at - from).Normalize();
        v3 right = up.Cross(forward).Normalize();
        v3 realUp = forward.Cross(right);
        return m44(v4(right.x(), realUp.x(), forward.x(), 0),
                   v4(right.y(), realUp.y(), forward.y(), 0),
                   v4(right.z(), realUp.z(), forward.z(), 0),
                   W);
    }

    inline static m44 LookAt(const v4& up, const v4& from, const v4& at)
    { return LookAt(up.xyz(), from.xyz(), at.xyz()); }

    inline static m44 Projection(fp width, fp height, fp fov, fp nearPlane, fp farPlane)
    {
        fp aspect = width / height;
        //fp tanFov = (fov * fp::Half).Tan();
        //fp w = fp::One / (tanFov * aspect);
        //fp h = fp::One / tanFov;
        fp q = 1 / (farPlane - nearPlane);

        return m44(v4(1, 0, 0, 0),
                   v4(0, aspect, 0, 0),
                   v4(0, 0, q, 1),
                   v4(0, 0, 0, 0));
    }

private:
    v4 m_r0;
    v4 m_r1;
    v4 m_r2;
    v4 m_r3;
};

}}
