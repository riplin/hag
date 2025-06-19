//Copyright 2023-Present riplin

#pragma once

#include <hag/math/fp/fp.h>

namespace Hag::Math {

class v2
{
public:
    inline v2() {}
    explicit inline v2(fp v) : m_x(v), m_y(v) {}
    explicit inline v2(fp x, fp y) : m_x(x), m_y(y) {}

    inline const fp& x() const { return m_x; }
    inline fp& x() { return m_x; }

    inline const fp& y() const { return m_y; }
    inline fp& y() { return m_y; }

    inline v2 xx() const { return v2(m_x, m_x); }
    inline v2 yy() const { return v2(m_y, m_y); }
    inline const v2& xy() const { return (*this); }
    inline v2 yx() const { return v2(m_y, m_x); }

    inline fp Dot(const v2& rhs) const
    { return m_x * rhs.m_x + m_y * rhs.m_y; }

    inline v2 Abs() const
    { return v2(m_x.Abs(), m_y.Abs()); }

    inline v2 Neg() const
    { return v2(m_x.Neg(), m_y.Neg()); }

    inline v2 Floor() const
    { return v2(m_x.Floor(), m_y.Floor()); }

    inline v2 Fraction() const
    { return v2(m_x.Fraction(), m_y.Fraction()); }

    inline const v2& operator=(const v2& rhs)
    { m_x = rhs.m_x; m_y = rhs.m_y; return (*this); }

    inline const v2& operator=(fp rhs)
    { m_x = rhs; m_y = rhs; return (*this); }

    inline bool operator==(const v2& rhs) const
    { return m_x == rhs.m_x && m_y == rhs.m_y; }

    inline bool operator!=(const v2& rhs) const
    { return m_x != rhs.m_x || m_y != rhs.m_y; }

    //do the operators >, >=, <, <= make sense?

    inline v2 operator+(const v2& rhs) const
    { return v2(m_x + rhs.m_x, m_y + rhs.m_y); }

    inline v2 operator+(fp rhs) const
    { return v2(m_x + rhs, m_y + rhs); }

    inline const v2& operator+=(const v2& rhs)
    { m_x += rhs.m_x; m_y += rhs.m_y; return (*this); }

    inline const v2& operator+=(fp rhs)
    { m_x += rhs; m_y += rhs; return (*this); }

    inline v2 operator-(const v2& rhs) const
    { return v2(m_x - rhs.m_x, m_y - rhs.m_y); }

    inline v2 operator-(fp rhs) const
    { return v2(m_x - rhs, m_y - rhs); }

    inline const v2& operator-=(const v2& rhs)
    { m_x -= rhs.m_x; m_y -= rhs.m_y; return (*this); }

    inline const v2& operator-=(fp rhs)
    { m_x -= rhs; m_y -= rhs; return (*this); }

    inline v2 operator*(const v2& rhs) const
    { return v2(m_x * rhs.m_x, m_y * rhs.m_y); }

    inline v2 operator*(fp rhs) const
    { return v2(m_x * rhs, m_y * rhs); }

    inline const v2& operator*=(const v2& rhs)
    { m_x *= rhs.m_x; m_y *= rhs.m_y; return (*this); }

    inline const v2& operator*=(fp rhs)
    { m_x *= rhs; m_y *= rhs; return (*this); }

    inline v2 operator/(const v2& rhs) const
    { return v2(m_x / rhs.m_x, m_y / rhs.m_y); }

    inline v2 operator/(fp rhs) const
    { return v2(m_x / rhs, m_y / rhs); }

    inline const v2& operator/=(const v2& rhs)
    { m_x /= rhs.m_x; m_y /= rhs.m_y; return (*this); }

    inline const v2& operator/=(fp rhs)
    { m_x /= rhs; m_y /= rhs; return (*this); }

    inline v2 operator%(const v2& rhs) const
    { return v2(m_x % rhs.m_x, m_y % rhs.m_y); }

    inline v2 operator%(fp rhs) const
    { return v2(m_x % rhs, m_y % rhs); }

    inline const v2& operator%=(const v2& rhs)
    { m_x %= rhs.m_x; m_y %= rhs.m_y; return (*this); }

    inline const v2& operator%=(fp rhs)
    { m_x %= rhs; m_y %= rhs; return (*this); }

private:
    fp m_x;
    fp m_y;
    //Padding?
};

inline v2 operator+(fp lhs, const v2& rhs)
{ return v2(lhs + rhs.x(), lhs + rhs.y()); }

inline v2 operator-(fp lhs, const v2& rhs)
{ return v2(lhs - rhs.x(), lhs - rhs.y()); }

inline v2 operator*(fp lhs, const v2& rhs)
{ return v2(lhs * rhs.x(), lhs * rhs.y()); }

inline v2 operator/(fp lhs, const v2& rhs)
{ return v2(lhs / rhs.x(), lhs / rhs.y()); }

inline v2 operator%(fp lhs, const v2& rhs)
{ return v2(lhs % rhs.x(), lhs % rhs.y()); }

}
