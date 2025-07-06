//Copyright 2025-Present riplin

#pragma once

#include <hag/math/flt/v2.h>

namespace Hag::Math::flt
{

class v3
{
public:
    inline v3() {}
    explicit inline v3(float v) : m_x(v), m_y(v), m_z(v) {}
    explicit inline v3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
    explicit inline v3(float x, const v2& yz) : m_x(x), m_y(yz.x()), m_z(yz.y()){}
    explicit inline v3(const v2& xy, float z) : m_x(xy.x()), m_y(xy.y()), m_z(z) {}

    inline const float& x() const { return m_x; }
    inline float& x() { return m_x; }

    inline const float& y() const { return m_y; }
    inline float& y() { return m_y; }

    inline const float& z() const { return m_z; }
    inline float& z() { return m_z; }

    inline v2 xx() const { return v2(m_x); }
    inline v2 xy() const { return v2(m_x, m_y); }
    inline v2 xz() const { return v2(m_x, m_z); }
    inline v2 yx() const { return v2(m_y, m_x); }
    inline v2 yy() const { return v2(m_y); }
    inline v2 yz() const { return v2(m_y, m_z); }
    inline v2 zx() const { return v2(m_z, m_x); }
    inline v2 zy() const { return v2(m_z, m_y); }
    inline v2 zz() const { return v2(m_z); }

    inline v3 xxx() const { return v3(m_x); }
    inline v3 xxy() const { return v3(m_x, m_x, m_y); }
    inline v3 xxz() const { return v3(m_x, m_x, m_z); }
    inline v3 xyx() const { return v3(m_x, m_y, m_x); }
    inline v3 xyy() const { return v3(m_x, m_y, m_y); }
    inline const v3& xyz() const { return (*this); }
    inline v3 xzx() const { return v3(m_x, m_z, m_x); }
    inline v3 xzy() const { return v3(m_x, m_z, m_y); }
    inline v3 xzz() const { return v3(m_x, m_z, m_z); }
    inline v3 yxx() const { return v3(m_y, m_x, m_x); }
    inline v3 yxy() const { return v3(m_y, m_x, m_y); }
    inline v3 yxz() const { return v3(m_y, m_x, m_z); }
    inline v3 yyx() const { return v3(m_y, m_y, m_x); }
    inline v3 yyy() const { return v3(m_y); }
    inline v3 yyz() const { return v3(m_y, m_y, m_z); }
    inline v3 yzx() const { return v3(m_y, m_z, m_x); }
    inline v3 yzy() const { return v3(m_y, m_z, m_y); }
    inline v3 yzz() const { return v3(m_y, m_z, m_z); }
    inline v3 zxx() const { return v3(m_z, m_x, m_x); }
    inline v3 zxy() const { return v3(m_z, m_x, m_y); }
    inline v3 zxz() const { return v3(m_z, m_x, m_z); }
    inline v3 zyx() const { return v3(m_z, m_y, m_x); }
    inline v3 zyy() const { return v3(m_z, m_y, m_y); }
    inline v3 zyz() const { return v3(m_z, m_y, m_z); }
    inline v3 zzx() const { return v3(m_z, m_z, m_x); }
    inline v3 zzy() const { return v3(m_z, m_z, m_y); }
    inline v3 zzz() const { return v3(m_z); }

    inline v3 Cross(const v3& rhs) const
    { return v3(m_y * rhs.m_z - m_z * rhs.m_y,
                m_z * rhs.m_x - m_x * rhs.m_z,
                m_x * rhs.m_y - m_y * rhs.m_x); }

    inline float Dot(const v3& rhs) const
    { return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z; }

    inline v3 Abs() const
    { return v3(fabs(m_x), fabs(m_y), fabs(m_z)); }

    inline v3 Neg() const
    { return v3(-m_x, -m_y, -m_z); }

    inline v3 Floor() const
    { return v3(floor(m_x), floor(m_y), floor(m_z)); }

    inline v3 Fraction() const
    { return v3(m_x - long(m_x), m_y - long(m_y), m_z - long(m_z)); }

    inline v3 Normalize() const
    { float invSqrt = 1 / sqrt(Dot(*this)); return v3(m_x * invSqrt, m_y * invSqrt, m_z * invSqrt); }

    inline const v3& operator=(const v3& rhs)
    { m_x = rhs.m_x; m_y = rhs.m_y; m_z = rhs.m_z; return (*this); }

    inline const v3& operator=(float rhs)
    { m_x = rhs; m_y = rhs; m_z = rhs; return (*this); }

    inline bool operator==(const v3& rhs) const
    { return issimilar(m_x, rhs.m_x) && issimilar(m_y, rhs.m_y) && issimilar(m_z, rhs.m_z); }

    inline bool operator!=(const v3& rhs) const
    { return !issimilar(m_x, rhs.m_x) || !issimilar(m_y, rhs.m_y) || !issimilar(m_z, rhs.m_z); }

    //do the operators >, >=, <, <= make sense?

    inline v3 operator+(const v3& rhs) const
    { return v3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z); }

    inline v3 operator+(float rhs) const
    { return v3(m_x + rhs, m_y + rhs, m_z + rhs); }

    inline const v3& operator+=(const v3& rhs)
    { m_x += rhs.m_x; m_y += rhs.m_y; m_z += rhs.m_z; return (*this); }

    inline const v3& operator+=(float rhs)
    { m_x += rhs; m_y += rhs; m_z += rhs; return (*this); }

    inline v3 operator-(const v3& rhs) const
    { return v3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z); }

    inline v3 operator-(float rhs) const
    { return v3(m_x - rhs, m_y - rhs, m_z - rhs); }

    inline const v3& operator-=(const v3& rhs)
    { m_x -= rhs.m_x; m_y -= rhs.m_y; m_z -= rhs.m_z; return (*this); }

    inline const v3& operator-=(float rhs)
    { m_x -= rhs; m_y -= rhs; m_z -= rhs; return (*this); }

    inline v3 operator*(const v3& rhs) const
    { return v3(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z); }

    inline v3 operator*(float rhs) const
    { return v3(m_x * rhs, m_y * rhs, m_z * rhs); }

    inline const v3& operator*=(const v3& rhs)
    { m_x *= rhs.m_x; m_y *= rhs.m_y; m_z *= rhs.m_z; return (*this); }

    inline const v3& operator*=(float rhs)
    { m_x *= rhs; m_y *= rhs; m_z *= rhs; return (*this); }

    inline v3 operator/(const v3& rhs) const
    { return v3(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z); }

    inline v3 operator/(float rhs) const
    { return v3(m_x / rhs, m_y / rhs, m_z / rhs); }

    inline const v3& operator/=(const v3& rhs)
    { m_x /= rhs.m_x; m_y /= rhs.m_y; m_z /= rhs.m_z; return (*this); }

    inline const v3& operator/=(float rhs)
    { m_x /= rhs; m_y /= rhs; m_z /= rhs; return (*this); }

    inline v3 operator%(const v3& rhs) const
    { return v3(fmod(m_x, rhs.m_x), fmod(m_y, rhs.m_y), fmod(m_z, rhs.m_z)); }

    inline v3 operator%(float rhs) const
    { return v3(fmod(m_x, rhs), fmod(m_y, rhs), fmod(m_z, rhs)); }

    inline const v3& operator%=(const v3& rhs)
    { m_x = fmod(m_x, rhs.m_x); m_y =fmod(m_y, rhs.m_y); m_z = fmod(m_z, rhs.m_z); return (*this); }

    inline const v3& operator%=(float rhs)
    { m_x = fmod(m_x, rhs); m_y = fmod(m_y, rhs); m_z = fmod(m_z, rhs); return (*this); }

private:
    float m_x;
    float m_y;
    float m_z;
    //Padding?
};

inline v3 operator+(float lhs, const v3& rhs)
{ return v3(lhs + rhs.x(), lhs + rhs.y(), lhs + rhs.z()); }

inline v3 operator-(float lhs, const v3& rhs)
{ return v3(lhs - rhs.x(), lhs - rhs.y(), lhs - rhs.z()); }

inline v3 operator*(float lhs, const v3& rhs)
{ return v3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z()); }

inline v3 operator/(float lhs, const v3& rhs)
{ return v3(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z()); }

inline v3 operator%(float lhs, const v3& rhs)
{ return v3(fmod(lhs, rhs.x()), fmod(lhs, rhs.y()), fmod(lhs, rhs.z())); }

}
