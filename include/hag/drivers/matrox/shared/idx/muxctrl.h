//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{
    
enum
{
    MultiplexControl = 0x19
};

}

typedef uint8_t MultiplexControl_t;

namespace MultiplexControl
{
    enum //Mask
    {
        ColorDepth = 0x07,          // Color depth. The mgamode field of the CRTCEXT3 VGA register overrides the ColorDepth field and
                                    // sets the DAC to VGA mode (when at ‘0’).
                                    // The following table shows the available color depths and their properties:
        Bits8p = 0x00,              // 000 = 8 bits/pixel (palettized) (default)
        Bits15p = 0x01,             // 001 = 15 bits/pixel (palettized) + 1-bit overlay
        Bits16p = 0x02,             // 010 = 16 bits/pixel (palettized)
        Bits24p = 0x03,             // 011 = 24 bits/pixel (packed, palettized)
        Bits32O8p = 0x04,           // 100 = 32 bits/pixel (24 bpp direct, 8 bpp overlay palettized)
        BitsV15h8p = 0x05,          // 101 = 16 bits/pixel 2G8V16 (15 bpp video direct, 8 bpp graphics palettized,
                                    //       video half-resolution)
        Bits16V16p = 0x06,          // 110 = 32 bits/pixel G16V16 (15 bpp video, 15 bpp graphics) One of the
                                    //       pixels is palettized (refer to videopal).
        Bits24p8x = 0x07,           // 111 = 32 bits/pixel (24 bpp palettized, 8 bpp unused)
        VideoPalette = 0x08,        // Palette source in G16V16 mode. In G16V16 mode, this bit indicates the source that
                                    // goes through the palette:
        GraphicsPalette = 0x00,     // 0 = graphics go through the palette
                                    // 1 = video goes through the palette
    };

    namespace Shift
    {
        enum
        {
            ColorDepth = 0x00,
            VideoPalette = 0x03,
        };
    }

}

}}}}