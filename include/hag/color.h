//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag
{

typedef uint16_t Color15_t;
typedef uint16_t Color16_t;
typedef uint32_t Color32_t;
namespace Color
{

namespace Bpp16
{
    enum
    {
        Black                   = 0x0000,
        Blue                    = 0x001F,
        Green                   = 0x07E0,
        LightGreen              = 0x7FEF,
        Lime                    = 0x6FE1,
        Cyan                    = 0x07FF,
        Red                     = 0xF800,
        Cherry                  = 0xD045,
        Pink                    = 0xFBEF,
        Magenta                 = 0xF81F,
        RoyalPurple             = 0x480D,
        Yellow                  = 0xFFE0,
        Cream                   = 0xFFDA,
        White                   = 0xFFFF,
        Orange                  = 0xFBE0,
        OrangeRed               = 0xFA20,
        BabyBlue                = 0x8E7E,
    };
}

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

namespace Gradient::Bpp32
{
    extern Color32_t Gold[5];
    extern Color32_t Silver[6];
}

}

}