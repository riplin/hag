//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Math { class fp; }}

Hag::Math::fp imulfp(Hag::Math::fp a, Hag::Math::fp b);
#pragma aux imulfp =            \
        "imul edx"              \
        "shrd eax, edx, 16"     \
        parm [eax] [edx] modify exact [eax edx] value [eax];

Hag::Math::fp imulint32(Hag::Math::fp a, int32_t b);
#pragma aux imulint32 =         \
        "imul edx"              \
        parm [eax] [edx] modify exact [eax edx] value [eax];

Hag::Math::fp idivfp(Hag::Math::fp a, Hag::Math::fp b);
#pragma aux idivfp =            \
        "mov edx, eax"          \
        "sar edx, 16"           \
        "shl eax, 16"           \
        "idiv ebx"              \
        parm [eax] [ebx] modify exact [eax edx] value [eax];

Hag::Math::fp idivint32(Hag::Math::fp a, int32_t b);
#pragma aux idivint32 =         \
        "cdq"                   \
        "idiv ebx"              \
        parm [eax] [ebx] modify exact [eax edx] value [eax];

Hag::Math::fp int32idiv(int32_t a, Hag::Math::fp b);
#pragma aux int32idiv =         \
        "xor eax, eax"          \
        "idiv ebx"              \
        parm [edx] [ebx] modify exact [eax edx] value [eax];

Hag::Math::fp imodfp(Hag::Math::fp a, Hag::Math::fp b);
#pragma aux imodfp =            \
        "mov edx, eax"          \
        "shl eax, 16"           \
        "sar edx, 16"           \
        "idiv ebx"              \
        parm [eax] [ebx] modify exact [eax edx] value [edx];

Hag::Math::fp imodint32(Hag::Math::fp a, int32_t b);
#pragma aux imodint32 =         \
        "shl ebx, 16"           \
        "mov edx, eax"          \
        "shl eax, 16"           \
        "sar edx, 16"           \
        "idiv ebx"              \
        parm [eax] [ebx] modify exact [eax edx] value [edx];

Hag::Math::fp int32imod(int32_t a, Hag::Math::fp b);
#pragma aux int32imod =         \
        "xor eax, eax"          \
        "idiv ebx"              \
        parm [edx] [ebx] modify exact [eax edx] value [edx];

namespace Hag { namespace Math {

class fp
{
public:
    inline fp() {}
    inline fp(int16_t val) { m_split.m_val = val; m_split.m_frac = 0; }
    explicit inline fp(int16_t val, int16_t frac) { m_split.m_val = val; m_split.m_frac = frac; }

    inline fp Floor() const { return fp(m_split.m_val); }
    inline fp Fraction() const { return ((*this) - Floor()); }
    
    inline int32_t RawTotal() const { return m_total; }
    inline int16_t RawCeil() const { return m_total > 0 ? (m_total + 0x0000FFFF) >> 16 : (m_total - 0x0000FFFF) >> 16; }
    inline int16_t RawFloor() const { return m_split.m_val; }
    inline int16_t RawFraction() const { return m_split.m_frac; }

    void ToString(char* result, int32_t maxDecimals) const;
    
    inline static fp FromTotal(int32_t total) { fp ret; ret.m_total = total; return ret; }
    inline fp Neg() const { return FromTotal(-m_total); }
    inline fp Abs() const { return m_total < 0 ? Neg() : *this; }
    inline fp ToRad() const { return ((*this) * Pi) / fp(180); }
    inline fp ToDeg() const { return ((*this) * fp(180)) / Pi; }

    fp Sqrt() const;
    fp Sin() const;
    fp Cos() const;
    fp Tan() const;

    inline static fp Divide(int32_t lhs, int32_t rhs)
    { int64_t l = lhs; return FromTotal(int32_t((l << 32) / (int64_t(rhs) << 16))); }

    inline fp operator+(fp rhs) const
    { return FromTotal(m_total + rhs.m_total); }
    
    inline fp operator+(int16_t rhs) const
    { return FromTotal(m_total + (int32_t(rhs) << 16)); }

    inline fp operator-(fp rhs) const
    { return FromTotal(m_total - rhs.m_total); }

    inline fp operator-(int16_t rhs) const
    { return FromTotal(m_total - (int32_t(rhs) << 16)); }
    
    fp operator*(fp rhs) const;
    fp operator*(int16_t rhs) const;
    fp operator/(fp rhs) const;
    fp operator/(int16_t rhs) const;
    fp operator%(fp rhs) const;
    fp operator%(int16_t rhs) const;

    inline fp operator+=(fp rhs)
    { m_total += rhs.m_total; return *this; }

    inline fp operator+=(int16_t rhs)
    { m_total += int32_t(rhs) << 16; return *this; }
    
    inline fp operator-=(fp rhs)
    { m_total -= rhs.m_total; return *this; }
    
    inline fp operator -=(int16_t rhs)
    { m_total -= int32_t(rhs) << 16; return *this; }

    fp operator*=(fp rhs);
    fp operator*=(int16_t rhs);
    fp operator/=(fp rhs);
    fp operator/=(int16_t rhs);
    fp operator%=(fp rhs);
    fp operator%=(int16_t rhs);

    inline bool operator<(fp rhs) const
    { return m_total < rhs.m_total; }
    
    inline bool operator<(int16_t rhs) const
    { return m_total < (int32_t(rhs) << 16); }

    inline bool operator>(fp rhs) const
    { return m_total > rhs.m_total; }
    
    inline bool operator>(int16_t rhs) const
    { return m_total > (int32_t(rhs) << 16); }

    inline bool operator<=(fp rhs) const
    { return m_total <= rhs.m_total; }

    inline bool operator<=(int16_t rhs) const
    { return m_total <= (int32_t(rhs) << 16); }
    
    inline bool operator>=(fp rhs) const
    { return m_total >= rhs.m_total; }

    inline bool operator>=(int16_t rhs) const
    { return m_total >= (int32_t(rhs) << 16); }
    
    inline bool operator==(fp rhs) const
    { return m_total == rhs.m_total; }

    inline bool operator==(int16_t rhs) const
    { return m_total == (int32_t(rhs) << 16); }
    
    inline bool operator!=(fp rhs) const
    { return m_total != rhs.m_total; }

    inline bool operator!=(int16_t rhs) const
    { return m_total != (int32_t(rhs) << 16); }

    inline fp operator=(fp rhs)
    { m_total = rhs.m_total; return *this; }

    inline fp operator=(int16_t rhs)
    { m_split.m_val = rhs; m_split.m_frac = 0; return *this; }
    
    static fp Pi;
    static fp HalfPi;
    static fp TwoPi;
    static fp MinusHalfPi;
    static fp MinusPi;
    static fp MinusTwoPi;
    static fp Zero;
    static fp Half;
    static fp One;
    static fp Two;
    static fp MinusHalf;
    static fp MinusOne;
    static fp MinusTwo;
    static fp OneThird;

private:
    
    union
    {
        int32_t m_total;
        struct
        {
            int16_t m_frac;
            int16_t m_val;
        } m_split;
    };
};

inline fp fp::operator*(fp rhs) const
//{ return FromTotal(int32_t((int64_t(m_total) * int64_t(rhs.m_total)) >> 16)); }
{ return imulfp((*this), rhs); }

inline fp fp::operator*(int16_t rhs) const
//{ return FromTotal(m_total * rhs); }
{ return imulint32((*this), rhs); }
    
inline fp fp::operator/(fp rhs) const
//{ return FromTotal(int32_t((int64_t(m_total) << 16) / rhs.m_total)); }
{ return idivfp((*this), rhs); }
    
inline fp fp::operator/(int16_t rhs) const
//{ return FromTotal(m_total / rhs); }
{ return idivint32((*this), rhs); }

inline fp fp::operator*=(fp rhs)
//{ (*this) = FromTotal(int32_t((int64_t(m_total) * int64_t(rhs.m_total)) >> 16)); return (*this); }
{ (*this) = imulfp((*this), rhs); return (*this); }
    
inline fp fp::operator*=(int16_t rhs)
//{ (*this) = FromTotal(m_total * rhs); return (*this); }
{ (*this) = imulint32((*this), rhs); return (*this); }

inline fp fp::operator/=(fp rhs)
//{ (*this) = FromTotal(int32_t((int64_t(m_total) << 16) / rhs.m_total)); return (*this); }
{ (*this) = idivfp((*this), rhs); return (*this); }
    
inline fp fp::operator/=(int16_t rhs)
//{ (*this) = FromTotal(m_total / rhs); return (*this); }
{ (*this) = idivint32((*this), rhs); return (*this); }

inline fp fp::operator%(fp rhs) const
{ return FromTotal(m_total % rhs.m_total); }
//{ return imodfp((*this), rhs); }

inline fp fp::operator%(int16_t rhs) const
{ return FromTotal(m_total % (int32_t(rhs) << 16)); }
//{ return imodint32((*this), rhs); }

inline fp fp::operator%=(fp rhs)
{ (*this) = FromTotal(m_total % rhs.m_total); return (*this); }
//{ (*this) = imodfp((*this), rhs); return (*this); }

inline fp fp::operator%=(int16_t rhs)
{ (*this) = FromTotal(m_total % (int32_t(rhs) << 16)); return (*this); }
//{ (*this) = imodint32((*this), rhs); return (*this); }

inline fp operator+(int16_t lhs, fp rhs)
{
    return fp(lhs) + rhs;
}

inline fp operator-(int16_t lhs, fp rhs)
{
    return fp(lhs) - rhs;
}

inline fp operator*(int16_t lhs, fp rhs)
{
    return imulint32(rhs, lhs);
}

inline fp operator/(int16_t lhs, fp rhs)
{
    return int32idiv(lhs, rhs);
}

inline fp operator%(int16_t lhs, fp rhs)
{
    return int32imod(lhs, rhs);
}

inline bool operator==(int16_t lhs, fp rhs)
{
    return fp(lhs) == rhs;
}

inline bool operator!=(int16_t lhs, fp rhs)
{
    return fp(lhs) != rhs;
}

}}
