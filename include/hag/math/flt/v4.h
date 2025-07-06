//Copyright 2025-Present riplin

#pragma once

#include <hag/math/flt/v3.h>
#include <hag/testing/log.h>

namespace Hag::Math::flt
{

class v4
{
public:
    inline v4() {}
    explicit inline v4(float v) : m_x(v), m_y(v), m_z(v), m_w(v) {}
    explicit inline v4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
    explicit inline v4(float x, const v3& yzw) : m_x(x), m_y(yzw.x()), m_z(yzw.y()), m_w(yzw.z()) {}
    explicit inline v4(const v3& xyz, float w) : m_x(xyz.x()), m_y(xyz.y()), m_z(xyz.z()), m_w(w) {}
    explicit inline v4(float x, float y, const v2& zw) : m_x(x), m_y(y), m_z(zw.x()), m_w(zw.y()) {}
    explicit inline v4(float x, const v2& yz, float w) : m_x(x), m_y(yz.x()), m_z(yz.y()), m_w(w) {}
    explicit inline v4(const v2& xy, float z, float w) : m_x(xy.x()), m_y(xy.y()), m_z(z), m_w(w) {}
    explicit inline v4(const v2& xy, const v2& zw) : m_x(xy.x()), m_y(xy.y()), m_z(zw.x()), m_w(zw.y()) {}


    inline const float& x() const { return m_x; }
    inline float& x() { return m_x; }

    inline const float& y() const { return m_y; }
    inline float& y() { return m_y; }

    inline const float& z() const { return m_z; }
    inline float& z() { return m_z; }

    inline const float& w() const { return m_w; }
    inline float& w() { return m_w; }

    inline v2 xx() const { return v2(m_x); }
    inline v2 xy() const { return v2(m_x, m_y); }
    inline v2 xz() const { return v2(m_x, m_z); }
    inline v2 xw() const { return v2(m_x, m_w); }
    inline v2 yx() const { return v2(m_y, m_x); }
    inline v2 yy() const { return v2(m_y); }
    inline v2 yz() const { return v2(m_y, m_z); }
    inline v2 yw() const { return v2(m_y, m_w); }
    inline v2 zx() const { return v2(m_z, m_x); }
    inline v2 zy() const { return v2(m_z, m_y); }
    inline v2 zz() const { return v2(m_z); }
    inline v2 zw() const { return v2(m_w, m_w); }
    inline v2 wx() const { return v2(m_w, m_x); }
    inline v2 wy() const { return v2(m_w, m_y); }
    inline v2 wz() const { return v2(m_w, m_z); }
    inline v2 ww() const { return v2(m_w); }

    inline v3 xxx() const { return v3(m_x); }
    inline v3 xxy() const { return v3(m_x, m_x, m_y); }
    inline v3 xxz() const { return v3(m_x, m_x, m_z); }
    inline v3 xxw() const { return v3(m_x, m_x, m_w); }
    inline v3 xyx() const { return v3(m_x, m_y, m_x); }
    inline v3 xyy() const { return v3(m_x, m_y, m_y); }
    inline v3 xyz() const { return v3(m_x, m_y, m_z); }
    inline v3 xyw() const { return v3(m_x, m_y, m_w); }
    inline v3 xzx() const { return v3(m_x, m_z, m_x); }
    inline v3 xzy() const { return v3(m_x, m_z, m_y); }
    inline v3 xzz() const { return v3(m_x, m_z, m_z); }
    inline v3 xzw() const { return v3(m_x, m_z, m_w); }
    inline v3 xwx() const { return v3(m_x, m_w, m_x); }
    inline v3 xwy() const { return v3(m_x, m_w, m_y); }
    inline v3 xwz() const { return v3(m_x, m_w, m_z); }
    inline v3 xww() const { return v3(m_x, m_w, m_w); }
    inline v3 yxx() const { return v3(m_y, m_x, m_x); }
    inline v3 yxy() const { return v3(m_y, m_x, m_y); }
    inline v3 yxz() const { return v3(m_y, m_x, m_z); }
    inline v3 yxw() const { return v3(m_y, m_x, m_w); }
    inline v3 yyx() const { return v3(m_y, m_y, m_x); }
    inline v3 yyy() const { return v3(m_y); }
    inline v3 yyz() const { return v3(m_y, m_y, m_z); }
    inline v3 yyw() const { return v3(m_y, m_y, m_w); }
    inline v3 yzx() const { return v3(m_y, m_z, m_x); }
    inline v3 yzy() const { return v3(m_y, m_z, m_y); }
    inline v3 yzz() const { return v3(m_y, m_z, m_z); }
    inline v3 yzw() const { return v3(m_y, m_z, m_w); }
    inline v3 ywx() const { return v3(m_y, m_w, m_x); }
    inline v3 ywy() const { return v3(m_y, m_w, m_y); }
    inline v3 ywz() const { return v3(m_y, m_w, m_z); }
    inline v3 yww() const { return v3(m_y, m_w, m_w); }
    inline v3 zxx() const { return v3(m_z, m_x, m_x); }
    inline v3 zxy() const { return v3(m_z, m_x, m_y); }
    inline v3 zxz() const { return v3(m_z, m_x, m_z); }
    inline v3 zxw() const { return v3(m_z, m_x, m_w); }
    inline v3 zyx() const { return v3(m_z, m_y, m_x); }
    inline v3 zyy() const { return v3(m_z, m_y, m_y); }
    inline v3 zyz() const { return v3(m_z, m_y, m_z); }
    inline v3 zyw() const { return v3(m_z, m_y, m_w); }
    inline v3 zzx() const { return v3(m_z, m_z, m_x); }
    inline v3 zzy() const { return v3(m_z, m_z, m_y); }
    inline v3 zzz() const { return v3(m_z); }
    inline v3 zzw() const { return v3(m_z, m_z, m_w); }
    inline v3 zwx() const { return v3(m_z, m_w, m_x); }
    inline v3 zwy() const { return v3(m_z, m_w, m_y); }
    inline v3 zwz() const { return v3(m_z, m_w, m_z); }
    inline v3 zww() const { return v3(m_z, m_w, m_w); }
    inline v3 wxx() const { return v3(m_w, m_x, m_x); }
    inline v3 wxy() const { return v3(m_w, m_x, m_y); }
    inline v3 wxz() const { return v3(m_w, m_x, m_z); }
    inline v3 wxw() const { return v3(m_w, m_x, m_w); }
    inline v3 wyx() const { return v3(m_w, m_y, m_x); }
    inline v3 wyy() const { return v3(m_w, m_y, m_y); }
    inline v3 wyz() const { return v3(m_w, m_y, m_z); }
    inline v3 wyw() const { return v3(m_w, m_y, m_w); }
    inline v3 wzx() const { return v3(m_w, m_z, m_x); }
    inline v3 wzy() const { return v3(m_w, m_z, m_y); }
    inline v3 wzz() const { return v3(m_w, m_z, m_z); }
    inline v3 wzw() const { return v3(m_w, m_z, m_w); }
    inline v3 wwx() const { return v3(m_w, m_w, m_x); }
    inline v3 wwy() const { return v3(m_w, m_w, m_y); }
    inline v3 wwz() const { return v3(m_w, m_w, m_z); }
    inline v3 www() const { return v3(m_w); }

    inline v4 xxxx() const { return v4(m_x); }
    inline v4 xxxy() const { return v4(m_x, m_x, m_x, m_y); }
    inline v4 xxxz() const { return v4(m_x, m_x, m_x, m_z); }
    inline v4 xxxw() const { return v4(m_x, m_x, m_x, m_w); }
    inline v4 xxyx() const { return v4(m_x, m_x, m_y, m_x); }
    inline v4 xxyy() const { return v4(m_x, m_x, m_y, m_y); }
    inline v4 xxyz() const { return v4(m_x, m_x, m_y, m_z); }
    inline v4 xxyw() const { return v4(m_x, m_x, m_y, m_w); }
    inline v4 xxzx() const { return v4(m_x, m_x, m_z, m_x); }
    inline v4 xxzy() const { return v4(m_x, m_x, m_z, m_y); }
    inline v4 xxzz() const { return v4(m_x, m_x, m_z, m_z); }
    inline v4 xxzw() const { return v4(m_x, m_x, m_z, m_w); }
    inline v4 xxwx() const { return v4(m_x, m_x, m_w, m_x); }
    inline v4 xxwy() const { return v4(m_x, m_x, m_w, m_y); }
    inline v4 xxwz() const { return v4(m_x, m_x, m_w, m_z); }
    inline v4 xxww() const { return v4(m_x, m_x, m_w, m_w); }
    inline v4 xyxx() const { return v4(m_x, m_y, m_x, m_x); }
    inline v4 xyxy() const { return v4(m_x, m_y, m_x, m_y); }
    inline v4 xyxz() const { return v4(m_x, m_y, m_x, m_z); }
    inline v4 xyxw() const { return v4(m_x, m_y, m_x, m_w); }
    inline v4 xyyx() const { return v4(m_x, m_y, m_y, m_x); }
    inline v4 xyyy() const { return v4(m_x, m_y, m_y, m_y); }
    inline v4 xyyz() const { return v4(m_x, m_y, m_y, m_z); }
    inline v4 xyyw() const { return v4(m_x, m_y, m_y, m_w); }
    inline v4 xyzx() const { return v4(m_x, m_y, m_z, m_x); }
    inline v4 xyzy() const { return v4(m_x, m_y, m_z, m_y); }
    inline v4 xyzz() const { return v4(m_x, m_y, m_z, m_z); }
    inline const v4& xyzw() const { return (*this); }
    inline v4 xywx() const { return v4(m_x, m_y, m_w, m_x); }
    inline v4 xywy() const { return v4(m_x, m_y, m_w, m_y); }
    inline v4 xywz() const { return v4(m_x, m_y, m_w, m_z); }
    inline v4 xyww() const { return v4(m_x, m_y, m_w, m_w); }
    inline v4 xzxx() const { return v4(m_x, m_z, m_x, m_x); }
    inline v4 xzxy() const { return v4(m_x, m_z, m_x, m_y); }
    inline v4 xzxz() const { return v4(m_x, m_z, m_x, m_z); }
    inline v4 xzxw() const { return v4(m_x, m_z, m_x, m_w); }
    inline v4 xzyx() const { return v4(m_x, m_z, m_y, m_x); }
    inline v4 xzyy() const { return v4(m_x, m_z, m_y, m_y); }
    inline v4 xzyz() const { return v4(m_x, m_z, m_y, m_z); }
    inline v4 xzyw() const { return v4(m_x, m_z, m_y, m_w); }
    inline v4 xzzx() const { return v4(m_x, m_z, m_z, m_x); }
    inline v4 xzzy() const { return v4(m_x, m_z, m_z, m_y); }
    inline v4 xzzz() const { return v4(m_x, m_z, m_z, m_z); }
    inline v4 xzzw() const { return v4(m_x, m_z, m_z, m_w); }
    inline v4 xzwx() const { return v4(m_x, m_z, m_w, m_x); }
    inline v4 xzwy() const { return v4(m_x, m_z, m_w, m_y); }
    inline v4 xzwz() const { return v4(m_x, m_z, m_w, m_z); }
    inline v4 xzww() const { return v4(m_x, m_z, m_w, m_w); }
    inline v4 xwxx() const { return v4(m_x, m_w, m_x, m_x); }
    inline v4 xwxy() const { return v4(m_x, m_w, m_x, m_y); }
    inline v4 xwxz() const { return v4(m_x, m_w, m_x, m_z); }
    inline v4 xwxw() const { return v4(m_x, m_w, m_x, m_w); }
    inline v4 xwyx() const { return v4(m_x, m_w, m_y, m_x); }
    inline v4 xwyy() const { return v4(m_x, m_w, m_y, m_y); }
    inline v4 xwyz() const { return v4(m_x, m_w, m_y, m_z); }
    inline v4 xwyw() const { return v4(m_x, m_w, m_y, m_w); }
    inline v4 xwzx() const { return v4(m_x, m_w, m_z, m_x); }
    inline v4 xwzy() const { return v4(m_x, m_w, m_z, m_y); }
    inline v4 xwzz() const { return v4(m_x, m_w, m_z, m_z); }
    inline v4 xwzw() const { return v4(m_x, m_w, m_z, m_w); }
    inline v4 xwwx() const { return v4(m_x, m_w, m_w, m_x); }
    inline v4 xwwy() const { return v4(m_x, m_w, m_w, m_y); }
    inline v4 xwwz() const { return v4(m_x, m_w, m_w, m_z); }
    inline v4 xwww() const { return v4(m_x, m_w, m_w, m_w); }
    inline v4 yxxx() const { return v4(m_y, m_x, m_x, m_x); }
    inline v4 yxxy() const { return v4(m_y, m_x, m_x, m_y); }
    inline v4 yxxz() const { return v4(m_y, m_x, m_x, m_z); }
    inline v4 yxxw() const { return v4(m_y, m_x, m_x, m_w); }
    inline v4 yxyx() const { return v4(m_y, m_x, m_y, m_x); }
    inline v4 yxyy() const { return v4(m_y, m_x, m_y, m_y); }
    inline v4 yxyz() const { return v4(m_y, m_x, m_y, m_z); }
    inline v4 yxyw() const { return v4(m_y, m_x, m_y, m_w); }
    inline v4 yxzx() const { return v4(m_y, m_x, m_z, m_x); }
    inline v4 yxzy() const { return v4(m_y, m_x, m_z, m_y); }
    inline v4 yxzz() const { return v4(m_y, m_x, m_z, m_z); }
    inline v4 yxzw() const { return v4(m_y, m_x, m_z, m_w); }
    inline v4 yxwx() const { return v4(m_y, m_x, m_w, m_x); }
    inline v4 yxwy() const { return v4(m_y, m_x, m_w, m_y); }
    inline v4 yxwz() const { return v4(m_y, m_x, m_w, m_z); }
    inline v4 yxww() const { return v4(m_y, m_x, m_w, m_w); }
    inline v4 yyxx() const { return v4(m_y, m_y, m_x, m_x); }
    inline v4 yyxy() const { return v4(m_y, m_y, m_x, m_y); }
    inline v4 yyxz() const { return v4(m_y, m_y, m_x, m_z); }
    inline v4 yyxw() const { return v4(m_y, m_y, m_x, m_w); }
    inline v4 yyyx() const { return v4(m_y, m_y, m_y, m_x); }
    inline v4 yyyy() const { return v4(m_y); }
    inline v4 yyyz() const { return v4(m_y, m_y, m_y, m_z); }
    inline v4 yyyw() const { return v4(m_y, m_y, m_y, m_w); }
    inline v4 yyzx() const { return v4(m_y, m_y, m_z, m_x); }
    inline v4 yyzy() const { return v4(m_y, m_y, m_z, m_y); }
    inline v4 yyzz() const { return v4(m_y, m_y, m_z, m_z); }
    inline v4 yyzw() const { return v4(m_y, m_y, m_z, m_w); }
    inline v4 yywx() const { return v4(m_y, m_y, m_w, m_x); }
    inline v4 yywy() const { return v4(m_y, m_y, m_w, m_y); }
    inline v4 yywz() const { return v4(m_y, m_y, m_w, m_z); }
    inline v4 yyww() const { return v4(m_y, m_y, m_w, m_w); }
    inline v4 yzxx() const { return v4(m_y, m_z, m_x, m_x); }
    inline v4 yzxy() const { return v4(m_y, m_z, m_x, m_y); }
    inline v4 yzxz() const { return v4(m_y, m_z, m_x, m_z); }
    inline v4 yzxw() const { return v4(m_y, m_z, m_x, m_w); }
    inline v4 yzyx() const { return v4(m_y, m_z, m_y, m_x); }
    inline v4 yzyy() const { return v4(m_y, m_z, m_y, m_y); }
    inline v4 yzyz() const { return v4(m_y, m_z, m_y, m_z); }
    inline v4 yzyw() const { return v4(m_y, m_z, m_y, m_w); }
    inline v4 yzzx() const { return v4(m_y, m_z, m_z, m_x); }
    inline v4 yzzy() const { return v4(m_y, m_z, m_z, m_y); }
    inline v4 yzzz() const { return v4(m_y, m_z, m_z, m_z); }
    inline v4 yzzw() const { return v4(m_y, m_z, m_z, m_w); }
    inline v4 yzwx() const { return v4(m_y, m_z, m_w, m_x); }
    inline v4 yzwy() const { return v4(m_y, m_z, m_w, m_y); }
    inline v4 yzwz() const { return v4(m_y, m_z, m_w, m_z); }
    inline v4 yzww() const { return v4(m_y, m_z, m_w, m_w); }
    inline v4 ywxx() const { return v4(m_y, m_w, m_x, m_x); }
    inline v4 ywxy() const { return v4(m_y, m_w, m_x, m_y); }
    inline v4 ywxz() const { return v4(m_y, m_w, m_x, m_z); }
    inline v4 ywxw() const { return v4(m_y, m_w, m_x, m_w); }
    inline v4 ywyx() const { return v4(m_y, m_w, m_y, m_x); }
    inline v4 ywyy() const { return v4(m_y, m_w, m_y, m_y); }
    inline v4 ywyz() const { return v4(m_y, m_w, m_y, m_z); }
    inline v4 ywyw() const { return v4(m_y, m_w, m_y, m_w); }
    inline v4 ywzx() const { return v4(m_y, m_w, m_z, m_x); }
    inline v4 ywzy() const { return v4(m_y, m_w, m_z, m_y); }
    inline v4 ywzz() const { return v4(m_y, m_w, m_z, m_z); }
    inline v4 ywzw() const { return v4(m_y, m_w, m_z, m_w); }
    inline v4 ywwx() const { return v4(m_y, m_w, m_w, m_x); }
    inline v4 ywwy() const { return v4(m_y, m_w, m_w, m_y); }
    inline v4 ywwz() const { return v4(m_y, m_w, m_w, m_z); }
    inline v4 ywww() const { return v4(m_y, m_w, m_w, m_w); }

    inline v4 zxxx() const { return v4(m_z, m_x, m_x, m_x); }
    inline v4 zxxy() const { return v4(m_z, m_x, m_x, m_y); }
    inline v4 zxxz() const { return v4(m_z, m_x, m_x, m_z); }
    inline v4 zxxw() const { return v4(m_z, m_x, m_x, m_w); }
    inline v4 zxyx() const { return v4(m_z, m_x, m_y, m_x); }
    inline v4 zxyy() const { return v4(m_z, m_x, m_y, m_y); }
    inline v4 zxyz() const { return v4(m_z, m_x, m_y, m_z); }
    inline v4 zxyw() const { return v4(m_z, m_x, m_y, m_w); }
    inline v4 zxzx() const { return v4(m_z, m_x, m_z, m_x); }
    inline v4 zxzy() const { return v4(m_z, m_x, m_z, m_y); }
    inline v4 zxzz() const { return v4(m_z, m_x, m_z, m_z); }
    inline v4 zxzw() const { return v4(m_z, m_x, m_z, m_w); }
    inline v4 zxwx() const { return v4(m_z, m_x, m_w, m_x); }
    inline v4 zxwy() const { return v4(m_z, m_x, m_w, m_y); }
    inline v4 zxwz() const { return v4(m_z, m_x, m_w, m_z); }
    inline v4 zxww() const { return v4(m_z, m_x, m_w, m_w); }
    inline v4 zyxx() const { return v4(m_z, m_y, m_x, m_x); }
    inline v4 zyxy() const { return v4(m_z, m_y, m_x, m_y); }
    inline v4 zyxz() const { return v4(m_z, m_y, m_x, m_z); }
    inline v4 zyxw() const { return v4(m_z, m_y, m_x, m_w); }
    inline v4 zyyx() const { return v4(m_z, m_y, m_y, m_x); }
    inline v4 zyyy() const { return v4(m_z, m_y, m_y, m_y); }
    inline v4 zyyz() const { return v4(m_z, m_y, m_y, m_z); }
    inline v4 zyyw() const { return v4(m_z, m_y, m_y, m_w); }
    inline v4 zyzx() const { return v4(m_z, m_y, m_z, m_x); }
    inline v4 zyzy() const { return v4(m_z, m_y, m_z, m_y); }
    inline v4 zyzz() const { return v4(m_z, m_y, m_z, m_z); }
    inline v4 zyzw() const { return v4(m_z, m_y, m_z, m_w); }
    inline v4 zywx() const { return v4(m_z, m_y, m_w, m_x); }
    inline v4 zywy() const { return v4(m_z, m_y, m_w, m_y); }
    inline v4 zywz() const { return v4(m_z, m_y, m_w, m_z); }
    inline v4 zyww() const { return v4(m_z, m_y, m_w, m_w); }
    inline v4 zzxx() const { return v4(m_z, m_z, m_x, m_x); }
    inline v4 zzxy() const { return v4(m_z, m_z, m_x, m_y); }
    inline v4 zzxz() const { return v4(m_z, m_z, m_x, m_z); }
    inline v4 zzxw() const { return v4(m_z, m_z, m_x, m_w); }
    inline v4 zzyx() const { return v4(m_z, m_z, m_y, m_x); }
    inline v4 zzyy() const { return v4(m_z, m_z, m_y, m_y); }
    inline v4 zzyz() const { return v4(m_z, m_z, m_y, m_z); }
    inline v4 zzyw() const { return v4(m_z, m_z, m_y, m_w); }
    inline v4 zzzx() const { return v4(m_z, m_z, m_z, m_x); }
    inline v4 zzzy() const { return v4(m_z, m_z, m_z, m_y); }
    inline v4 zzzz() const { return v4(m_z); }
    inline v4 zzzw() const { return v4(m_z, m_z, m_z, m_w); }
    inline v4 zzwx() const { return v4(m_z, m_z, m_w, m_x); }
    inline v4 zzwy() const { return v4(m_z, m_z, m_w, m_y); }
    inline v4 zzwz() const { return v4(m_z, m_z, m_w, m_z); }
    inline v4 zzww() const { return v4(m_z, m_z, m_w, m_w); }
    inline v4 zwxx() const { return v4(m_z, m_w, m_x, m_x); }
    inline v4 zwxy() const { return v4(m_z, m_w, m_x, m_y); }
    inline v4 zwxz() const { return v4(m_z, m_w, m_x, m_z); }
    inline v4 zwxw() const { return v4(m_z, m_w, m_x, m_w); }
    inline v4 zwyx() const { return v4(m_z, m_w, m_y, m_x); }
    inline v4 zwyy() const { return v4(m_z, m_w, m_y, m_y); }
    inline v4 zwyz() const { return v4(m_z, m_w, m_y, m_z); }
    inline v4 zwyw() const { return v4(m_z, m_w, m_y, m_w); }
    inline v4 zwzx() const { return v4(m_z, m_w, m_z, m_x); }
    inline v4 zwzy() const { return v4(m_z, m_w, m_z, m_y); }
    inline v4 zwzz() const { return v4(m_z, m_w, m_z, m_z); }
    inline v4 zwzw() const { return v4(m_z, m_w, m_z, m_w); }
    inline v4 zwwx() const { return v4(m_z, m_w, m_w, m_x); }
    inline v4 zwwy() const { return v4(m_z, m_w, m_w, m_y); }
    inline v4 zwwz() const { return v4(m_z, m_w, m_w, m_z); }
    inline v4 zwww() const { return v4(m_z, m_w, m_w, m_w); }

    inline v4 wxxx() const { return v4(m_w, m_x, m_x, m_x); }
    inline v4 wxxy() const { return v4(m_w, m_x, m_x, m_y); }
    inline v4 wxxz() const { return v4(m_w, m_x, m_x, m_z); }
    inline v4 wxxw() const { return v4(m_w, m_x, m_x, m_w); }
    inline v4 wxyx() const { return v4(m_w, m_x, m_y, m_x); }
    inline v4 wxyy() const { return v4(m_w, m_x, m_y, m_y); }
    inline v4 wxyz() const { return v4(m_w, m_x, m_y, m_z); }
    inline v4 wxyw() const { return v4(m_w, m_x, m_y, m_w); }
    inline v4 wxzx() const { return v4(m_w, m_x, m_z, m_x); }
    inline v4 wxzy() const { return v4(m_w, m_x, m_z, m_y); }
    inline v4 wxzz() const { return v4(m_w, m_x, m_z, m_z); }
    inline v4 wxzw() const { return v4(m_w, m_x, m_z, m_w); }
    inline v4 wxwx() const { return v4(m_w, m_x, m_w, m_x); }
    inline v4 wxwy() const { return v4(m_w, m_x, m_w, m_y); }
    inline v4 wxwz() const { return v4(m_w, m_x, m_w, m_z); }
    inline v4 wxww() const { return v4(m_w, m_x, m_w, m_w); }
    inline v4 wyxx() const { return v4(m_w, m_y, m_x, m_x); }
    inline v4 wyxy() const { return v4(m_w, m_y, m_x, m_y); }
    inline v4 wyxz() const { return v4(m_w, m_y, m_x, m_z); }
    inline v4 wyxw() const { return v4(m_w, m_y, m_x, m_w); }
    inline v4 wyyx() const { return v4(m_w, m_y, m_y, m_x); }
    inline v4 wyyy() const { return v4(m_w, m_y, m_y, m_y); }
    inline v4 wyyz() const { return v4(m_w, m_y, m_y, m_z); }
    inline v4 wyyw() const { return v4(m_w, m_y, m_y, m_w); }
    inline v4 wyzx() const { return v4(m_w, m_y, m_z, m_x); }
    inline v4 wyzy() const { return v4(m_w, m_y, m_z, m_y); }
    inline v4 wyzz() const { return v4(m_w, m_y, m_z, m_z); }
    inline v4 wyzw() const { return v4(m_w, m_y, m_z, m_w); }
    inline v4 wywx() const { return v4(m_w, m_y, m_w, m_x); }
    inline v4 wywy() const { return v4(m_w, m_y, m_w, m_y); }
    inline v4 wywz() const { return v4(m_w, m_y, m_w, m_z); }
    inline v4 wyww() const { return v4(m_w, m_y, m_w, m_w); }
    inline v4 wzxx() const { return v4(m_w, m_z, m_x, m_x); }
    inline v4 wzxy() const { return v4(m_w, m_z, m_x, m_y); }
    inline v4 wzxz() const { return v4(m_w, m_z, m_x, m_z); }
    inline v4 wzxw() const { return v4(m_w, m_z, m_x, m_w); }
    inline v4 wzyx() const { return v4(m_w, m_z, m_y, m_x); }
    inline v4 wzyy() const { return v4(m_w, m_z, m_y, m_y); }
    inline v4 wzyz() const { return v4(m_w, m_z, m_y, m_z); }
    inline v4 wzyw() const { return v4(m_w, m_z, m_y, m_w); }
    inline v4 wzzx() const { return v4(m_w, m_z, m_z, m_x); }
    inline v4 wzzy() const { return v4(m_w, m_z, m_z, m_y); }
    inline v4 wzzz() const { return v4(m_w, m_z, m_z, m_z); }
    inline v4 wzzw() const { return v4(m_w, m_z, m_z, m_w); }
    inline v4 wzwx() const { return v4(m_w, m_z, m_w, m_x); }
    inline v4 wzwy() const { return v4(m_w, m_z, m_w, m_y); }
    inline v4 wzwz() const { return v4(m_w, m_z, m_w, m_z); }
    inline v4 wzww() const { return v4(m_w, m_z, m_w, m_w); }
    inline v4 wwxx() const { return v4(m_w, m_w, m_x, m_x); }
    inline v4 wwxy() const { return v4(m_w, m_w, m_x, m_y); }
    inline v4 wwxz() const { return v4(m_w, m_w, m_x, m_z); }
    inline v4 wwxw() const { return v4(m_w, m_w, m_x, m_w); }
    inline v4 wwyx() const { return v4(m_w, m_w, m_y, m_x); }
    inline v4 wwyy() const { return v4(m_w, m_w, m_y, m_y); }
    inline v4 wwyz() const { return v4(m_w, m_w, m_y, m_z); }
    inline v4 wwyw() const { return v4(m_w, m_w, m_y, m_w); }
    inline v4 wwzx() const { return v4(m_w, m_w, m_z, m_x); }
    inline v4 wwzy() const { return v4(m_w, m_w, m_z, m_y); }
    inline v4 wwzz() const { return v4(m_w, m_w, m_z, m_z); }
    inline v4 wwzw() const { return v4(m_w, m_w, m_z, m_w); }
    inline v4 wwwx() const { return v4(m_w, m_w, m_w, m_x); }
    inline v4 wwwy() const { return v4(m_w, m_w, m_w, m_y); }
    inline v4 wwwz() const { return v4(m_w, m_w, m_w, m_z); }
    inline v4 wwww() const { return v4(m_w); }

    inline v4 Cross(const v4& rhs) const
    { return v4(m_y * rhs.m_z - m_z * rhs.m_y,
                m_z * rhs.m_x - m_x * rhs.m_z,
                m_x * rhs.m_y - m_y * rhs.m_x,
                Zero); }

    inline float Dot(const v4& rhs) const
    { return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z + m_w * rhs.m_w; }

    inline v4 Abs() const
    { return v4(fabs(m_x), fabs(m_y), fabs(m_z), fabs(m_w)); }

    inline v4 Neg() const
    { return v4(-m_x, -m_y, -m_z, -m_w); }

    inline v4 Floor() const
    { return v4(floor(m_x), floor(m_y), floor(m_z), floor(m_w)); }

    inline v4 Fraction() const
    { return v4(m_x - long(m_x), m_y - long(m_y), m_z - long(m_z), m_w - long(m_w)); }

    inline const v4& operator=(const v4& rhs)
    { m_x = rhs.m_x; m_y = rhs.m_y; m_z = rhs.m_z; m_w = rhs.m_w; return (*this); }

    inline const v4& operator=(float rhs)
    { m_x = rhs; m_y = rhs; m_z = rhs; m_w = rhs; return (*this); }

    inline bool operator==(const v4& rhs) const
    { return issimilar(m_x, rhs.m_x) && issimilar(m_y, rhs.m_y) && issimilar(m_z, rhs.m_z) && issimilar(m_w, rhs.m_w); }

    inline bool operator!=(const v4& rhs) const
    { return !issimilar(m_x, rhs.m_x) || !issimilar(m_y, rhs.m_y) || !issimilar(m_z, rhs.m_z) || !issimilar(m_w, rhs.m_w); }

    //do the operators >, >=, <, <= make sense?

    inline v4 operator+(const v4& rhs) const
    { return v4(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z, m_w + rhs.m_w); }

    inline v4 operator+(float rhs) const
    { return v4(m_x + rhs, m_y + rhs, m_z + rhs, m_w + rhs); }

    inline const v4& operator+=(const v4& rhs)
    { m_x += rhs.m_x; m_y += rhs.m_y; m_z += rhs.m_z; m_w += rhs.m_w; return (*this); }

    inline const v4& operator+=(float rhs)
    { m_x += rhs; m_y += rhs; m_z += rhs; m_w += rhs; return (*this); }

    inline v4 operator-(const v4& rhs) const
    { return v4(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z, m_w - rhs.m_w); }

    inline v4 operator-(float rhs) const
    { return v4(m_x - rhs, m_y - rhs, m_z - rhs, m_w - rhs); }

    inline const v4& operator-=(const v4& rhs)
    { m_x -= rhs.m_x; m_y -= rhs.m_y; m_z -= rhs.m_z; m_w -= rhs.m_w; return (*this); }

    inline const v4& operator-=(float rhs)
    { m_x -= rhs; m_y -= rhs; m_z -= rhs; m_w -= rhs; return (*this); }

    inline v4 operator*(const v4& rhs) const
    { return v4(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z, m_w * rhs.m_w); }

    inline v4 operator*(float rhs) const
    { return v4(m_x * rhs, m_y * rhs, m_z * rhs, m_w * rhs); }

    inline const v4& operator*=(const v4& rhs)
    { m_x *= rhs.m_x; m_y *= rhs.m_y; m_z *= rhs.m_z; m_w *= rhs.m_w; return (*this); }

    inline const v4& operator*=(float rhs)
    { m_x *= rhs; m_y *= rhs; m_z *= rhs; m_w *= rhs; return (*this); }

    inline v4 operator/(const v4& rhs) const
    { return v4(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z, m_w / rhs.m_w); }

    inline v4 operator/(float rhs) const
    { return v4(m_x / rhs, m_y / rhs, m_z / rhs, m_w / rhs); }

    inline const v4& operator/=(const v4& rhs)
    { m_x /= rhs.m_x; m_y /= rhs.m_y; m_z /= rhs.m_z; m_w /= rhs.m_w; return (*this); }

    inline const v4& operator/=(float rhs)
    { m_x /= rhs; m_y /= rhs; m_z /= rhs; m_w /= rhs; return (*this); }

    inline v4 operator%(const v4& rhs) const
    { return v4(fmod(m_x, rhs.m_x), fmod(m_y, rhs.m_y), fmod(m_z, rhs.m_z), fmod(m_w, rhs.m_w)); }

    inline v4 operator%(float rhs) const
    { return v4(fmod(m_x, rhs), fmod(m_y, rhs), fmod(m_z, rhs), fmod(m_w, rhs)); }

    inline const v4& operator%=(const v4& rhs)
    { m_x = fmod(m_x, rhs.m_x); m_y = fmod(m_y, rhs.m_y); m_z = fmod(m_z, rhs.m_z); m_w = fmod(m_w, rhs.m_w); return (*this); }

    inline const v4& operator%=(float rhs)
    { m_x = fmod(m_x, rhs); m_y = fmod(m_y, rhs); m_z = fmod(m_z, rhs); m_w = fmod(m_w, rhs); return (*this); }

    inline void Log(const char* name)
    {
        LOG("math::flt", "%s : %0.4f, %0.4f, %0.4f", name, m_x, m_y, m_z, m_w);
    }

private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};

inline v4 operator+(float lhs, const v4& rhs)
{ return v4(lhs + rhs.x(), lhs + rhs.y(), lhs + rhs.z(), lhs + rhs.w()); }

inline v4 operator-(float lhs, const v4& rhs)
{ return v4(lhs - rhs.x(), lhs - rhs.y(), lhs - rhs.z(), lhs - rhs.w()); }

inline v4 operator*(float lhs, const v4& rhs)
{ return v4(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z(), lhs * rhs.w()); }

inline v4 operator/(float lhs, const v4& rhs)
{ return v4(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z(), lhs / rhs.w()); }

inline v4 operator%(float lhs, const v4& rhs)
{ return v4(fmod(lhs, rhs.x()), fmod(lhs, rhs.y()), fmod(lhs, rhs.z()), fmod(lhs, rhs.w())); }

}
