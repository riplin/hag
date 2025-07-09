//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FogTable = 0x160 // to 0x1dc
    };
}

// fogTable Register
// The fogTable register is used to implement fog functions in Banshee. The fogTable register is a 64-entry
// lookup table consisting of 8-bit fog blending factors and 8-bit ∆fog blending values. The ∆fog blending
// values are the difference between successive fog blending factors in fogTable and are used to blend
// between fogTable entries. Note that the ∆fog blending factors are stored in 6.2 format, while the fog
// blending factors are stored in 8.0 format. For most applications, the 6.2 format ∆fog blending factors will
// have the two LSBs set to 0x0, with the six MSBs representing the difference between successive fog
// blending factors. Also note that as a result of the 6.2 format for the ∆fog blending factors, the difference
// between successive fog blending factors cannot exceed 63. When storing the fog blending factors, the sum
// of each fog blending factor and ∆fog blending factor pair must not exceed 255. When loading fogTable,
// two fog table entries must be written concurrently in a 32-bit word. A total of 32 32-bit PCI writes are
// required to load the entire fogTable register.

typedef uint32_t FogTable_t;
namespace FogTable
{
    enum
    {
        DeltaFog2N = 0x000000ff, // 7:0 FogTable[2n] ∆Fog blending factor
        Fog2N = 0x0000ff00, // 15:8 FogTable[2n] Fog blending factor
        DeltaFog2np1 = 0x00ff0000, // 23:16 FogTable[2n+1] ∆Fog blending factor
        Fog2Np1 = 0xff000000 // 31:24 FogTable[2n+1] Fog blending factor
    };

    namespace Shift
    {
        enum
        {
            DeltaFog2N = 0x00,
            Fog2N = 0x08,
            DeltaFog2np1 = 0x10,
            Fog2Np1 = 0x18
        };
    }
}

}