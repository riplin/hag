//Copyright 2023-Present riplin

#pragma once

#include <stdint.h>

//These are defined to clearly communicate the number of bits that are expected.
typedef unsigned char uint1_t;
typedef unsigned char uint2_t;
typedef unsigned char uint3_t;
typedef unsigned char uint4_t;
typedef unsigned char uint5_t;
typedef unsigned char uint6_t;
typedef unsigned char uint7_t;
typedef unsigned short uint9_t;
typedef unsigned short uint10_t;
typedef unsigned short uint11_t;
typedef unsigned short uint12_t;
typedef unsigned short uint13_t;
typedef unsigned short uint14_t;
typedef unsigned short uint15_t;
typedef unsigned int uint17_t;
typedef unsigned int uint18_t;
typedef unsigned int uint19_t;
typedef unsigned int uint20_t;

namespace Hag
{
    template<typename T> T min(T a, T b) { return a < b ? a : b; }
    template<typename T> T max(T a, T b) { return a > b ? a : b; }
    template<typename T> T abs(T a) { return a < T(0) ? -a : a; }
}
