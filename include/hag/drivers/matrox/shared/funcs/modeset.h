//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Function
{

namespace ModeSetting
{
    typedef uint8_t BitsPerPixel_t;
    namespace BitsPerPixel
    {
        enum
        {
            Bpp1 = 0x01,
            Bpp2 = 0x02,
            Bpp4 = 0x04,
            Bpp8 = 0x08,
            Bpp15 = 0x0F,
            Bpp16 = 0x10,
            Bpp24 = 0x18,
            Bpp32 = 0x20
        };
    }

    typedef uint16_t Flags_t;
    namespace Flags
    {
        enum
        {
            Mode = 0x01,                //Mode type is text or graphics
            Text = 0x01,
            Graphics = 0x00,

            Chromacity = 0x02,          //Monochrome or color
            Monochrome = 0x02,
            Color = 0x00,

            MemoryOrganization = 0x04,  //Sequential or planar memory organization, only applies to graphical modes
            Sequential = 0x00,
            Planar = 0x04,
        };
    }

    typedef uint8_t RefreshRate_t;
    namespace RefreshRate
    {
        enum
        {
            DontCare = 0,
            R60Hz = 60,
            R70Hz = 70
        };
    }

    typedef uint8_t SetVideoError_t;
    namespace SetVideoError
    {
        enum
        {
            Success = 0x00,
            SystemNotInitialized = 0x01,
            UnknownMode = 0x02,
            InsufficientVideoMemory = 0x03,
            NotSupportedByRamdac = 0x04,
            NotSupportedByMonitor = 0x05,
        };
    };

    SetVideoError_t HasVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare);
    SetVideoError_t SetVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare, bool clearDisplay = true);
}

}}}}