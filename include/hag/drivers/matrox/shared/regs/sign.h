//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    Sign = 0x1C58,
    SignCommit = 0x1D58
};

}

typedef uint32_t Sign_t;

// Note: Writing to this register when DrawingControl’s SignZero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ is written to SignZero prior to accessing Sign.

namespace Sign
{

    enum //Mask
    {
        SignDeltaYMinusDeltaX = 0x00000001,     // Sign of delta y minus delta x. This bit is shared with scanleft. It is defined for LINE
                                                // drawing only and specifies the major axis. This bit is automatically initialized during
                                                // AUTOLINE operations.
        MajorAxisY = 0x00000000,                // 0 = major axis is y
        MajorAxisX = 0x00000001,                // 1 = major axis is x
        HorizontalScanDirection = 0x00000001,   // Horizontal scan direction left (1) vs. right (0). This bit is shared with sdydxl and
                                                // affects TRAPs and BLITs; scanleft is set according to the x scanning direction in a BLIT
        ScanRight = 0x00000000,                 // Normally, this bit is always programmed to zero except for BITBLT when bltmod =
        ScanLeft = 0x00000001,                  // BFCOL (see DWGCTL on page 4-49). For TRAP drawing, this bit must be set to 0 (scan right).
        SignOfDeltaX = 0x00000002,              // Sign of delta x (line draw or left trapezoid edge). The sdxl field specifies the x direc-
                                                // tion for a line draw (opcod = LINE) or the x direction when plotting the left edge in a
                                                // filled trapezoid draw. This bit is automatically initialized during AUTOLINE operation.
        DeltaXPositive = 0x00000000,            // 0 = Delta X is Positive.
        DeltaXNegative = 0x00000002,            // 1 = Delta X is Negative.
        SignOfDeltaY = 0x00000004,              // Sign of delta y. The sdy field specifies the y direction of the destination address. This
                                                // bit is automatically initialized during AUTOLINE operations. This bit should be pro-
                                                // grammed to zero for TRAP.
        DeltaYPositive = 0x00000000,            // 0 = Delta Y is Positive.
        DeltaYNegative = 0x00000004,            // 1 = Delta Y is Negative.
        SignOfDeltaXRight = 0x00000020,         // Sign of delta x (right trapezoid edge). The sdxr field specifies the x direction of the
                                                // right edge of a filled trapezoid.
        DeltaXRightPositive = 0x00000000,       // 0 = Delta X Right is Positive.
        DeltaXRightNegative = 0x00000020        // 1 = Delta X Right is Negative.
    };

    namespace Shift
    {
        enum
        {
            SignDeltaYMinusDeltaX = 0x00,
            HorizontalScanDirection = 0x00,
            SignOfDeltaX = 0x01,
            SignOfDeltaY = 0x02,
            SignOfDeltaXRight = 0x05

        };
    }
}

}
