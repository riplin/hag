//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        ColorBufferAddress = 0x1ec,
        AuxBufferAddress = 0x1f4
    };
}

// colBufferAddr
// The colBufferAddr register defines the base address of the color buffer. The the address must be 16-byte
// aligned, so colBufferAddr[3:0] are unused.

// auxBufferAddr
// The auxBufferAddr register defines the base address of the auxiliary buffer. The existence and enabling of
// the depth or the alpha auxiliary buffers is established within the fbzMode register. AuxBufferAddr must
// be 16 byte aligned, so auxBufferAddr[3:0] are unused.

typedef uint32_t BufferAddress_t;
namespace BufferAddress
{
    enum
    {
        BaseAddress = 0x00fffff0
    };

    namespace Shift
    {
        enum
        {
            BaseAddress = 0x00
        };
    }
}

}