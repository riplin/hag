//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    XYStartAddress = 0x1C40,
    XYEndAddress = 0x1C44,
    XYStartAddressCommit = 0x1D40,
    XYEndAddressCommit = 0x1D44
};

}

typedef uint32_t XYAddress_t;

// The XYSTRT register is not a physical register. It is simply an alternate way to load
// registers AR5, AR6, XDST, and YDST.
// The XYSTRT register is only used for LINE and AUTOLINE. XYSTRT does not
// need to be initialized for polylines because all the registers affected by XYSTRT are
// updated to the endpoint of the vector at the end of the AUTOLINE.
// When XYSTRT is written, the following registers are affected:
//      x_start<15:0> --> xdst<15:0>
//      x_start<15:0> --> ar5<17:0> (sign extended)
//      y_start<15:0> --> ydst<21:0> (sign extended) 0 --> sellin
//      y_start<15:0> --> ar6<17:0> (sign extended)
// The x_start field contains the x coordinate of the starting point of the vector. It is a
// 16-bit signed value in two’s complement notation.
// The y_start field contains the y coordinate of the starting point of the vector. This
// coordinate is always xy (this means that in order to use the XYSTRT register the lin-
// earizer must be used). It is a 16-bit signed value in two’s complement notation.

// The XYEND register is not a physical register. It is simply an alternate way to load
// registers AR0 and AR2.
// The XYEND register is only used for AUTOLINE drawing.
// When XYEND is written, the following registers are affected:
//      x_end<15:0> --> ar0<17:0> (sign extended)
//      y_end<15:0> --> ar2<17:0> (sign extended)
// The x_end field contains the x coordinate of the end point of the vector. It is a 16-bit
// signed value in two’s complement notation.
// The y_end field contains the y coordinate of the end point of the vector. It is a 16-bit
// signed value in two’s complement notation.


namespace XYAddress
{

    enum //Mask
    {
        XValue = 0x0000FFFF,
        YValue = 0xFFFF0000
    };

    namespace Shift
    {
        enum
        {
            XValue = 0x00,
            YValue = 0x10
        };
    }
}

}}}
