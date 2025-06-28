//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        DestinationBaseAddress = 0x010,
        SourceBaseAddress = 0x034
    };
}

// Bits(23:0) of these registers contain the addresses of the pixels at x=0, y=0 on the source and destination
// surfaces in frame-buffer memory. Bit(31) of each register is reserved and must be zero.
// The srcBaseAddr register is used only for screen-to-screen blts. For host-blts, the alignment of the initial
// pixel sent from the host is determined by the x entry in the srcXY register.
// For YUYV422 and UYVY422 surfaces, the base address must be dword aligned. Thus bits(1:0) of srcBaseAddr must be 0.

typedef uint32_t BaseAddress_t;
namespace BaseAddress
{

    enum
    {
        Address = 0x00ffffff
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