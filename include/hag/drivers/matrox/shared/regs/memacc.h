//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    MemoryAccess = 0x1C04,
    MemoryAccessCommit = 0x1D04
};

}

typedef uint32_t MemoryAccess_t;

namespace MemoryAccess
{

    enum //Mask
    {
        PixelWidth =                0x00000003,     // Pixel width. Specifies the normal pixel width for drawing.
        Width8bpp =                 0x00000000,     // PW8      8 bpp
        Width16bpp =                0x00000001,     // PW16    16 bpp
        Width32bpp =                0x00000002,     // PW32    32 bpp
        Width24bpp =                0x00000003,     // PW24    24 bpp
        JEDECPowerUpSequence =      0x00004000,     // JEDEC power-up sequence.
        MemorySeqRegSet8Refresh =   0x00000000,     // 0 = Memory sequencer performs the mode register set, followed by eight refreshes.
        MemorySeqJEDEC =            0x00004000,     // 1 = Memory sequencer performs the JEDEC sequence (eight refreshes followed by
                                                    //     the mode register set).
        MemoryReset =               0x00008000,     // Resets the RAM. When this bit is set to ‘1’, the memory sequencer will generate a
                                                    // power-up cycle to the RAM.
                                                    // Caution: Refer to Section 5.3.3 on page 5-21 for instructions on when to use
                                                    // this field. The memreset field must always be set to ‘0’ except under
                                                    // specific conditions which occur during the reset sequence.
        Dithering =                 0x40000000,     // Enable/disable dithering.
        DitheringEnabled =          0x00000000,     // 0 = Dithering is performed on unformatted ILOAD, ZI, and I trapezoids.
        DitheringDisabled =         0x40000000,     // 1 = Dithering is disabled.
        Dither555 =                 0x80000000,     // Dither 5:5:5 mode. This field should normally be set to 0, except for 16 bit/pixel
                                                    // configurations, when it affects dithering and shading.
        Dither565 =                 0x00000000      // The pixel format is 5:6:5
    };

    namespace Shift
    {
        enum
        {
            PixelWidth = 0x00,
            JEDECPowerUpSequence = 0x0E,
            MemoryReset = 0x0F,
            Dithering = 0x1E,
            Dither555 = 0x1F
        };
    }
}

}}}
