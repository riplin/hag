//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    PlaneWriteMask = 0x1C1C,
    PlaneWriteMaskCommit = 0x1D1C
};

}

typedef uint32_t PlaneWriteMask_t;

namespace PlaneWriteMask
{

    enum //Mask
    {
        Mask = 0xFFFFFFFF           // Plane write mask. Plane(s) to be protected during any write operations. The plane
                                    // write mask is not used for z cycles, or for direct write access (all planes are written in
                                    // this case).
                                    // • 0 = inhibit write
                                    // • 1 = permit write
                                    // The bits from the plnwrmsk<31:0> register are output on the MDQ<31:0> signal and
                                    // also on MDQ<63:32>. In 8 and 16 bit/pixel configurations, all bits in pln-
                                    // wrmsk<31:0> are used, so the mask information must be replicated on all bytes. In 24
                                    // bits/pixel, the plane masking feature is limited to the case of all three colors having the
                                    // same mask. The four bytes of plnwrmsk must be identical.
                                    // Refer to ‘Pixel Format’ on page 5-18 for the the definition of the slice in each mode.
    };

    namespace Shift
    {
        enum
        {
            Mask = 0x00
        };
    }
}

}}}
