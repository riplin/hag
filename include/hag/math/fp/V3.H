//Copyright 2023-Present riplin

#pragma once

#include <hag/math/fp/v2.h>

namespace Hag { namespace Math {

class v3
{
public:
    inline v3() {}
    explicit inline v3(fp v) : m_x(v), m_y(v), m_z(v) {}
    explicit inline v3(fp x, fp y, fp z) : m_x(x), m_y(y), m_z(z) {}
    explicit inline v3(fp x, const v2& yz) : m_x(x), m_y(yz.x()), m_z(yz.y()){}
    explicit inline v3(const v2& xy, fp z) : m_x(xy.x()), m_y(xy.y()), m_z(z) {}

    inline const fp& x() const { return m_x; }
    inline fp& x() { return m_x; }

    inline const fp& y() const { return m_y; }
    inline fp& y() { return m_y; }

    inline const fp& z() const { return m_z; }
    inline fp& z() { return m_z; }

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

    inline fp Dot(const v3& rhs) const
    { return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z; }

    inline v3 Abs() const
    { return v3(m_x.Abs(), m_y.Abs(), m_z.Abs()); }

    inline v3 Neg() const
    { return v3(m_x.Neg(), m_y.Neg(), m_z.Neg()); }

    inline v3 Floor() const
    { return v3(m_x.Floor(), m_y.Floor(), m_z.Floor()); }

    inline v3 Fraction() const
    { return v3(m_x.Fraction(), m_y.Fraction(), m_z.Fraction()); }

    inline v3 Normalize() const
    { fp invSqrt = 1 / Dot(*this).Sqrt(); return v3(m_x * invSqrt, m_y * invSqrt, m_z * invSqrt); }

    inline const v3& operator=(const v3& rhs)
    { m_x = rhs.m_x; m_y = rhs.m_y; m_z = rhs.m_z; return (*this); }

    inline const v3& operator=(fp rhs)
    { m_x = rhs; m_y = rhs; m_z = rhs; return (*this); }

    inline bool operator==(const v3& rhs) const
    { return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z; }

    inline bool operator!=(const v3& rhs) const
    { return m_x != rhs.m_x || m_y != rhs.m_y || m_z != rhs.m_z; }

    //do the operators >, >=, <, <= make sense?

    inline v3 operator+(const v3& rhs) const
    { return v3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z); }

    inline v3 operator+(fp rhs) const
    { return v3(m_x + rhs, m_y + rhs, m_z + rhs); }

    inline const v3& operator+=(const v3& rhs)
    { m_x += rhs.m_x; m_y += rhs.m_y; m_z += rhs.m_z; return (*this); }

    inline const v3& operator+=(fp rhs)
    { m_x += rhs; m_y += rhs; m_z += rhs; return (*this); }

    inline v3 operator-(const v3& rhs) const
    { return v3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z); }

    inline v3 operator-(fp rhs) const
    { return v3(m_x - rhs, m_y - rhs, m_z - rhs); }

    inline const v3& operator-=(const v3& rhs)
    { m_x -= rhs.m_x; m_y -= rhs.m_y; m_z -= rhs.m_z; return (*this); }

    inline const v3& operator-=(fp rhs)
    { m_x -= rhs; m_y -= rhs; m_z -= rhs; return (*this); }

    inline v3 operator*(const v3& rhs) const
    { return v3(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z); }

    inline v3 operator*(fp rhs) const
    { return v3(m_x * rhs, m_y * rhs, m_z * rhs); }

    inline const v3& operator*=(const v3& rhs)
    { m_x *= rhs.m_x; m_y *= rhs.m_y; m_z *= rhs.m_z; return (*this); }

    inline const v3& operator*=(fp rhs)
    { m_x *= rhs; m_y *= rhs; m_z *= rhs; return (*this); }

    inline v3 operator/(const v3& rhs) const
    { return v3(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z); }

    inline v3 operator/(fp rhs) const
    { return v3(m_x / rhs, m_y / rhs, m_z / rhs); }

    inline const v3& operator/=(const v3& rhs)
    { m_x /= rhs.m_x; m_y /= rhs.m_y; m_z /= rhs.m_z; return (*this); }

    inline const v3& operator/=(fp rhs)
    { m_x /= rhs; m_y /= rhs; m_z /= rhs; return (*this); }

    inline v3 operator%(const v3& rhs) const
    { return v3(m_x % rhs.m_x, m_y % rhs.m_y, m_z % rhs.m_z); }

    inline v3 operator%(fp rhs) const
    { return v3(m_x % rhs, m_y % rhs, m_z % rhs); }

    inline const v3& operator%=(const v3& rhs)
    { m_x %= rhs.m_x; m_y %= rhs.m_y; m_z %= rhs.m_z; return (*this); }

    inline const v3& operator%=(fp rhs)
    { m_x %= rhs; m_y %= rhs; m_z %= rhs; return (*this); }

private:
    fp m_x;
    fp m_y;
    fp m_z;
    //Padding?
};

inline v3 operator+(fp lhs, const v3& rhs)
{ return v3(lhs + rhs.x(), lhs + rhs.y(), lhs + rhs.z()); }

inline v3 operator-(fp lhs, const v3& rhs)
{ return v3(lhs - rhs.x(), lhs - rhs.y(), lhs - rhs.z()); }

inline v3 operator*(fp lhs, const v3& rhs)
{ return v3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z()); }

inline v3 operator/(fp lhs, const v3& rhs)
{ return v3(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z()); }

inline v3 operator%(fp lhs, const v3& rhs)
{ return v3(lhs % rhs.x(), lhs % rhs.y(), lhs % rhs.z()); }

}}
