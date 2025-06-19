//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    XYStartAddress = 0x1C40,
    XYEndAddress = 0x1C44,
    XLeftRight = 0x1C84,
    XLeft = 0x1CA8,
    XRight = 0x1CAC,
    XDestination = 0x1CB0,
    YDestinationAndLength = 0x1C88,
    YDestination = 0x1C90,
    XYStartAddressCommit = 0x1D40,
    XYEndAddressCommit = 0x1D44,
    XLeftRightCommit = 0x1D84,
    XLeftCommit = 0x1DA8,
    XRightCommit = 0x1DAC,
    XDestinationCommit = 0x1DB0,
    YDestinationAndLengthCommit = 0x1D88,
    YDestinationCommit = 0x1D90,
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

// The FXBNDRY register is not a physical register. It is simply an alternate way to load
// the FXRIGHT and FXLEFT registers.
// Filled object x left coordinate. Refer to the FXLEFT register for a detailed description.
// Filled object x right coordinate. See the FXRIGHT register on page 4-60.

typedef uint32_t XLeftRight_t;

namespace XLeftRight
{
    enum //Mask
    {
        Left = 0x0000FFFF,
        Right = 0xFFFF0000
    };

    namespace Shift
    {
        enum
        {
            Left = 0x00,
            Right = 0x10
        };
    }
}

// Filled object x left coordinate. The fxleft field contains the x coordinate (in pixels) of
// the left boundary of any filled object being drawn. It is a 16-bit signed value in two’s
// complement notation.
// • The fxleft field is not used for line drawing.
// • During filled trapezoid drawing, fxleft is updated during the left edge scan.
// • During a BLIT operation, fxleft is static, and specifies the left pixel boundary of the
// area being written to.

typedef uint16_t XLeft_t;

namespace XLeft
{
    enum //Mask
    {
        Value = 0xFFFF
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

// Filled object x right coordinate. The fxright field contains the x coordinate (in pixels)
// of the right boundary of any filled object being drawn. It is a 16-bit signed value in
// two’s complement notation.
// • The fxright field is not used for line drawing.
// • During filled trapezoid drawing, fxright is updated during the right edge scan.
// • During a BLIT operation, fxright is static, and specifies the right pixel boundary of
// the area being written to.

typedef uint16_t XRight_t;

namespace XRight
{
    enum //Mask
    {
        Value = 0xFFFF
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

// The x coordinate of destination address. The xdst field contains the running x coordi-
// nate of the destination address. It is a 16-bit signed value in two’s complement nota-
// tion.
// • Before starting a vector draw, xdst must be loaded with the x coordinate of the start-
// ing point of the vector. At the end of a vector, xdst contains the address of the last
// pixel of the vector. This can also be done by accessing the XYSTRT register.
// • This register does not require initialization for polyline operations.
// • For BLITs, this register is automatically loaded from fxleft (see FXLEFT on page
// 4-59) and fxright (see FXRIGHT on page 4-60), and no initial value must be
// loaded.
// • For trapezoids with depth, this register is automatically loaded from fxleft. For trap-
// ezoids without depth, xdst will be loaded with the larger of fxleft or cxleft, and an
// initial value must not be loaded. (See CXLEFT on page 4-29.)

typedef uint16_t XDestination_t;

namespace XDestination
{
    enum //Mask
    {
        Value = 0xFFFF
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

// The YDSTLEN register is not a physical register. It is simply an alternate way to load
// the YDST and LEN registers.
// Length. See the LEN register on page 4-63.
// The y destination value. See the YDST register on page 4-80. The yval field can be
// used to load the YDST register in xy format. In this case the valid range -32768 to
// +32767 (16-bit signed) for YDST is respected.
// ydst<21:0> <= sign extension (yval<31:16> )
// For the linear format, yval does not contain enough bits, so YDST must be used
// directly.

typedef uint32_t YDestinationAndLength_t;

namespace YDestinationAndLength
{
    enum //Mask
    {
        Length = 0x0000FFFF,
        YValue = 0xFFFF0000,
    };

    namespace Shift
    {
        enum
        {
            Length = 0x00,
            YValue = 0x10
        };
    }
}

// The y destination. The ydst field contains the current y coordinate (in pixels) of the
// destination address as a signed value in two’s complement notation. Two formats are
// supported: linear format and xy format. The current format is selected by ylin (see
// PITCH on page 4-68).
// When xy format is used (ylin=0), ydst represents the y coordinate of the address. The
// valid range is -32768 to +32767 (16-bit signed). The xy value is always converted to a
// linear value before being used.
// When linear format is used (ylin=1), ydst must be programmed as follows:
// ydst <-- (Y coordinate) * PITCH >> 5
// The y coordinate range is from -32768 to +32767 (16-bit signed) and the pitch range is
// from 32 to 2048. Pitch is also a multiple of 32.
// • Before starting a vector draw, ydst must be loaded with the y coordinate of the start-
// ing point of the vector. This can be done by accessing the XYSTRT register. This
// register does not require initialization for polyline operations.
// • Before starting a BLIT, ydst must be loaded with the y coordinate of the starting cor-
// ner of the destination rectangle.
// • For trapezoids, this register must be loaded with the y coordinate of the first scanned
// line of the trapezoid.
// • To load the texture color palette, ydst must be loaded with the position in the color
// palette (0 to 255) at which the texture color palette will begin loading.

// Selected line. The sellin field is used to perform the dithering, patterning, and trans-
// parency functions. During linearization, this field is loaded with the three LSBs of
// ydst. If no linearization occurs, then those bits must be initialized correctly if one of
// the above-mentioned functions is to be used.

typedef uint32_t YDestination_t;

namespace YDestination
{
    enum //Mask
    {
        Value = 0x003FFFFF,
        SelectedLine = 0xE0000000
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            SelectedLine = 0x1D
        };
    }
}

}
