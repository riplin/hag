//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        LeftOverlayBufferAddress = 0x250,
        RightOverlayBufferAddress = 0x254
    };
}

// leftOverlayBuf Register
// Starting address of left or Monocular buffer address for overlay display. For video overlay, the start
// address needs to be aligned on a 32-bit boundary for YUV 422 pixel format and a 64-bit boundary for
// YUV 411 pixel format. This register is sampled at the end of vertical retrace.

// rightOverlayBuf Register
// Starting address of right buffer address for overlay display. For video overlay, the start address needs to
// be aligned on a 32-bit boundary for YUV 422 pixel format and a 64-bit boundary for YUV 411 pixel
// format. This register is only used for stereo buffering. This register is sampled at the end of vertical
// retrace.

typedef uint32_t OverlayBufferAddress_t;
namespace OverlayBufferAddress
{
    enum
    {
        Address = 0x0fffffff
    };

    namespace Shift
    {
        enum
        {
            Address = 0x00
        };
    }
}

}