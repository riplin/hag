//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        Stipple = 0x140
    };
}

// stipple Register
// The stipple register specifies a mask which is used to enable individual pixel writes to the RGB and depth
// buffers. See the stipple functionality description in the fbzMode register description for more
// information.

typedef uint32_t Stipple_t;
namespace Stipple
{
    enum
    {
        Value = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

}