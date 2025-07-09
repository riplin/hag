//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiTrianglesOut = 0x25c
    };
}

// fbiTrianglesOut Register
// The fbiTriangles register is a 24-bit counter which is incremented for each triangle processed by the
// Banshee triangle walking engine. Triangles which are backface culled in the triangle setup unit do not
// increment fbiTrianglesOut. fbiTrianglesOut is reset to 0x0 on power-up reset, and is also reset to 0x0
// when a ‘1’ is written to nopCMD bit(1).

typedef uint32_t FbiTrianglesOut_t;
namespace FbiTrianglesOut
{
    enum
    {
        Count = 0x00ffffff
    };

    namespace Shift
    {
        enum
        {
            Count = 0x00
        };
    }
}

}