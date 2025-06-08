//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    Status = 0x1E14
};

}

typedef uint32_t Status_t;

namespace Status
{

    enum //Mask
    {
        PickInterruptPending = 0x00000004,  // Pick interrupt pending. When set to ‘1’, indicates that a pick interrupt has occurred.
                                            // This bit is cleared through the pickiclr bit (see ICLEAR on page 4-61) or upon soft
                                            // or hard reset.
        VSync = 0x00000008,                 // VSYNC status. Set to ‘1’ during the VSYNC period. This bit follows the VSYNC signal.
        VSyncPending = 0x00000010,          // VSYNC interrupt pending. When set to ‘1’, indicates that a VSYNC interrupt has
                                            // occurred. (This bit is a copy of the crtcintCRT field of the INSTS0 VGA register).
                                            // This bit is cleared through the vintclr bit of CRTC11 or upon hard reset.
        VLinePending = 0x00000020,          // Vertical line interrupt pending. When set to ‘1’, indicates that the vertical line counter
                                            // has reached the value of the vertical interrupt line count. See the CRTC18 register
                                            // on page 4-125. This bit is cleared through the vlineiclr bit (see ICLEAR on page
                                            // 4-61) or upon soft or hard reset.
        ExtInterruptPending = 0x00000040,   // External interrupt pending. When set to ‘1’, indicates that the external interrupt line is
                                            // driven. This bit is cleared by conforming to the interrupt clear protocol of the external
                                            // device that drive the EXTINT/ line. After a hard reset, the state of this bit is unknown
                                            // (as indicated by the question mark in the ‘Reset Value’ above), as it depends on the
                                            // state of the EXTINT/ pin during the hard reset.
        DrawingEngineBusy = 0x00010000      // Drawing engine status. Set to ‘1’ when the drawing engine is busy (a busy condition
                                            // will be maintained until the BFIFO is empty, the drawing engine is finished with the
                                            // last drawing command, and the memory controller has completed the last memory access).
    };

    namespace Shift
    {
        enum
        {
            PickInterruptPending = 0x02,
            VSync = 0x03,
            VSyncPending = 0x04,
            VLinePending = 0x05,
            ExtInterruptPending = 0x06,
            DrawingEngineBusy = 0x10,
        };
    }
}

}}}
