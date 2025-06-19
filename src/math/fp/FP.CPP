//Copyright 2023-Present riplin

#include <hag/math/fp/fpmath.h>

namespace Hag::Math {

fp fp::Pi          = fp::Divide( 314159265, 100000000);
fp fp::HalfPi      = fp::Divide( 314159265, 200000000);
fp fp::TwoPi       = fp::Divide( 314159265,  50000000);
fp fp::MinusPi     = fp::Divide(-314159265, 100000000);
fp fp::MinusHalfPi = fp::Divide(-314159265, 200000000);
fp fp::MinusTwoPi  = fp::Divide(-314159265,  50000000);
fp fp::Zero        = fp( 0);
fp fp::Half        = fp::Divide( 1, 2);
fp fp::One         = fp( 1);
fp fp::Two         = fp( 2);
fp fp::MinusHalf   = fp::Divide(-1, 2);
fp fp::MinusOne    = fp(-1);
fp fp::MinusTwo    = fp(-2);
fp fp::OneThird    = fp::Divide( 1, 3);

fp fp::Sqrt() const
{
    fp val = FromTotal(m_total);
    bool invert = false;
    int32_t iter = 16;
    
    if (val < Zero)
        return MinusOne;
    
    if (val == Zero || val == One)
        return val;

    if (val < One && val.RawTotal() > 6)
    {
        invert = true;
        val = One / val;
    }

    if (val > One)
    {
        int32_t s = val.RawTotal();
        iter = 0;
        while (s > 0)
        {
            s >>= 2;
            ++iter;
        }
    }

    int32_t l = (val.RawTotal() >> 1) + 1;
    for (int32_t i = 0; i < iter; ++i)
    {
        l = (l + (val / FromTotal(l)).RawTotal()) >> 1;
    }

    fp ret = FromTotal(l);
    return invert ? One / ret : ret;
}

extern int32_t s_sinTable[];

fp fp::Sin() const
{
    bool neg = false;
    fp ranged = (*this) % TwoPi;
    if (ranged < Zero)
    {
        ranged += TwoPi;
    }
    if (ranged >= Pi)
    {
        neg = true;
        ranged -= Pi;
    }
    if (ranged >= HalfPi)
    {
        ranged = HalfPi - (ranged - HalfPi);
    }
    
    uint32_t idx = ranged.RawTotal() >> 2;
    
    int32_t val0 = s_sinTable[idx];
    int32_t val1 = s_sinTable[idx+1];
    int32_t vdelta = ((val1 - val0) * (ranged.RawTotal() & 0x3)) >> 2;
    
    fp val = FromTotal(val0 + vdelta);

    if (neg)
        val = val.Neg();
    
    return val;
}

fp fp::Cos() const
{
    return ((*this) + HalfPi).Sin();
}

fp fp::Tan() const
{
    return Sin() / Cos();
}

void fp::ToString(char* result, int32_t maxDecimals) const
{
    fp val = FromTotal(m_total);
    int32_t ndec = 0, slen = 0;
    char tmp[12] = {0};

    const int64_t one = int64_t(1) << 32;
    const int64_t mask = one - 1;

    if (maxDecimals == -1)
        maxDecimals = 2; 
    else if (maxDecimals == -2)
        maxDecimals = 15;
    
    if (val < 0)
    {
        result[slen++] = '-';
        val *= -1;
    }

    int32_t ival = val.RawFloor();

    do
    {
        tmp[ndec++] = '0' + (ival % 10);
        ival /= 10;
    } while (ival != 0);

    while (ndec > 0)
        result[slen++] = tmp[--ndec];
    result[slen++] = '.';

    int64_t frac = (int64_t(val.RawFraction()) << 16) & mask;
    do
    {
        frac = (frac & mask) * 10;
        result[slen++] = '0' + ((frac >> 32) % 10);
        ndec++;
    } while (frac != 0 && ndec < maxDecimals);

    if (ndec > 1 && result[slen - 1] == '0')
        result[slen - 1] = '\0';
    else
        result[slen] = '\0';
}

}
