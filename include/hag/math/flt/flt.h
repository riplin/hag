//Copyright 2025-Present riplin

#pragma once

#include <cmath>
#include <limits>
#include <algorithm>
#include <hag/types.h>

namespace Hag::Math::flt
{

extern float Pi;
extern float HalfPi;
extern float TwoPi;
extern float MinusHalfPi;
extern float MinusPi;
extern float MinusTwoPi;
extern float Zero;
extern float Half;
extern float One;
extern float Two;
extern float MinusHalf;
extern float MinusOne;
extern float MinusTwo;
extern float OneThird;

inline float ToRad(float deg) { return (deg * Pi) / 180; }
inline float ToDeg(float rad) { return (rad * 180) / Pi; }

inline bool issimilar(double x, double y)
{
    double maxXYOne = std::max( { 1.0, std::fabs(x) , std::fabs(y) } ) ;
    return std::fabs(x - y) <= std::numeric_limits<double>::epsilon() * maxXYOne;
}

inline bool issimilar(float x, float y)
{
    float maxXYOne = std::max( { 1.0f, std::fabs(x) , std::fabs(y) } ) ;
    return std::fabs(x - y) <= std::numeric_limits<float>::epsilon() * maxXYOne;
}

}