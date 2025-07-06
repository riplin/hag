//Copyright 2025-Present riplin

#pragma once

#include <math.h>
#include <hag/math/flt/flt.h>

namespace Hag::Math::flt
{

class v2
{
public:
    inline v2() {}
    explicit inline v2(float v) : m_x(v), m_y(v) {}
    explicit inline v2(float x, float y) : m_x(x), m_y(y) {}

    inline const float& x() const { return m_x; }
    inline float& x() { return m_x; }

    inline const float& y() const { return m_y; }
    inline float& y() { return m_y; }

    inline v2 xx() const { return v2(m_x, m_x); }
    inline v2 yy() const { return v2(m_y, m_y); }
    inline const v2& xy() const { return (*this); }
    inline v2 yx() const { return v2(m_y, m_x); }

    inline float Dot(const v2& rhs) const
    { return m_x * rhs.m_x + m_y * rhs.m_y; }

    inline v2 Abs() const
    { return v2(fabs(m_x), fabs(m_y)); }

    inline v2 Neg() const
    { return v2(-m_x, -m_y); }

    inline v2 Floor() const
    { return v2(floor(m_x), floor(m_y)); }

    inline v2 Fraction() const
    { return v2( m_x - long(m_x), m_y - long(m_y)); }

    inline const v2& operator=(const v2& rhs)
    { m_x = rhs.m_x; m_y = rhs.m_y; return (*this); }

    inline const v2& operator=(float rhs)
    { m_x = rhs; m_y = rhs; return (*this); }

    inline bool operator==(const v2& rhs) const
    { return issimilar(m_x, rhs.m_x) && issimilar(m_y, rhs.m_y); }

    inline bool operator!=(const v2& rhs) const
    { return !issimilar(m_x, rhs.m_x) || !issimilar(m_y, rhs.m_y); }

    //do the operators >, >=, <, <= make sense?

    inline v2 operator+(const v2& rhs) const
    { return v2(m_x + rhs.m_x, m_y + rhs.m_y); }

    inline v2 operator+(float rhs) const
    { return v2(m_x + rhs, m_y + rhs); }

    inline const v2& operator+=(const v2& rhs)
    { m_x += rhs.m_x; m_y += rhs.m_y; return (*this); }

    inline const v2& operator+=(float rhs)
    { m_x += rhs; m_y += rhs; return (*this); }

    inline v2 operator-(const v2& rhs) const
    { return v2(m_x - rhs.m_x, m_y - rhs.m_y); }

    inline v2 operator-(float rhs) const
    { return v2(m_x - rhs, m_y - rhs); }

    inline const v2& operator-=(const v2& rhs)
    { m_x -= rhs.m_x; m_y -= rhs.m_y; return (*this); }

    inline const v2& operator-=(float rhs)
    { m_x -= rhs; m_y -= rhs; return (*this); }

    inline v2 operator*(const v2& rhs) const
    { return v2(m_x * rhs.m_x, m_y * rhs.m_y); }

    inline v2 operator*(float rhs) const
    { return v2(m_x * rhs, m_y * rhs); }

    inline const v2& operator*=(const v2& rhs)
    { m_x *= rhs.m_x; m_y *= rhs.m_y; return (*this); }

    inline const v2& operator*=(float rhs)
    { m_x *= rhs; m_y *= rhs; return (*this); }

    inline v2 operator/(const v2& rhs) const
    { return v2(m_x / rhs.m_x, m_y / rhs.m_y); }

    inline v2 operator/(float rhs) const
    { return v2(m_x / rhs, m_y / rhs); }

    inline const v2& operator/=(const v2& rhs)
    { m_x /= rhs.m_x; m_y /= rhs.m_y; return (*this); }

    inline const v2& operator/=(float rhs)
    { m_x /= rhs; m_y /= rhs; return (*this); }

    inline v2 operator%(const v2& rhs) const
    { return v2(fmod(m_x, rhs.m_x), fmod(m_y, rhs.m_y)); }

    inline v2 operator%(float rhs) const
    { return v2(fmod(m_x, rhs), fmod(m_y, rhs)); }

    inline const v2& operator%=(const v2& rhs)
    { m_x = fmod(m_x, rhs.m_x); m_y = fmod(m_y, rhs.m_y); return (*this); }

    inline const v2& operator%=(float rhs)
    { m_x = fmod(m_x, rhs); m_y = fmod(m_y, rhs); return (*this); }

private:
    float m_x;
    float m_y;
    //Padding?
};

inline v2 operator+(float lhs, const v2& rhs)
{ return v2(lhs + rhs.x(), lhs + rhs.y()); }

inline v2 operator-(float lhs, const v2& rhs)
{ return v2(lhs - rhs.x(), lhs - rhs.y()); }

inline v2 operator*(float lhs, const v2& rhs)
{ return v2(lhs * rhs.x(), lhs * rhs.y()); }

inline v2 operator/(float lhs, const v2& rhs)
{ return v2(lhs / rhs.x(), lhs / rhs.y()); }

inline v2 operator%(float lhs, const v2& rhs)
{ return v2(fmod(lhs, rhs.x()), fmod(lhs, rhs.y())); }

}
