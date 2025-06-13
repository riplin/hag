//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag {

typedef uint16_t Color15_t;
typedef uint16_t Color16_t;
typedef uint32_t Color32_t;
namespace Color
{

namespace Bpp32
{
    enum
    {
        Black                   = 0xFF000000,
        Blue                    = 0xFF0000FF,
        Green                   = 0xFF00FF00,
        LightGreen              = 0xFF7FFF7F,
        Lime                    = 0xFF69FE08,
        Cyan                    = 0xFF00FFFF,
        Red                     = 0xFFFF0000,
        Cherry                  = 0xFFD20A2E,
        Pink                    = 0xFFFF7F7F,
        Magenta                 = 0xFFFF00FF,
        RoyalPurple             = 0xFF4B006E,
        Yellow                  = 0xFFFFFF00,
        Cream                   = 0xFFFDFBD4,
        White                   = 0xFFFFFFFF,
        Orange                  = 0xFFFF7F00,
        OrangeRed               = 0xFFFF4500,
        BabyBlue                = 0xFF89CFF0,
    };
}

namespace Gradient { namespace Bpp32
{
    extern Color32_t Gold[5];
    extern Color32_t Silver[6];
}}

}}