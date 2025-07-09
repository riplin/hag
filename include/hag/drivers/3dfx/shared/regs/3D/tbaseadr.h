//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        TextureBaseAddress = 0x30c,
        TextureBaseAddress1 = 0x310,
        TextureBaseAddress2 = 0x314,
        TextureBaseAddress3_8 = 0x318
    };
}

// texBaseAddr, texBaseAddr1, texBaseAddr2, and texBaseAddr38 Registers
// The texBaseAddr register specifies the starting texture memory address for accessing a texture. It is used
// for both rendering and texture downloading. Calculation of the texBaseAddr is described in the Texture
// Memory Access section. Selection of the base address is a function of tmultibaseaddr and LODBI.

// texBaseAddr[23:4] indicates the base address of the texture in 16-bytes units. If the texture is tiled
// (texBaseAddr[0]=1), then texBaseAddr[31:25] indicate the tile stride.

// texBaseAddr1, texBaseAddr2, texBaseAddr38 indicate the base addresses of lods 1, 2 and 3-8 in 16
// byte units, if tmultibaseaddr=1.

typedef uint32_t TextureBaseAddress_t;
namespace TextureBaseAddress
{
    enum
    {
        MemoryType = 0x00000001, // 0 texmemtype Texture Memory type (0=linear, 1=tiled)
        MemoryLinear = 0x00000000,
        MemoryTiled = 0x00000001,

        BaseAddress = 0x00fffff0, // 23:4 texbaseaddr Texture Memory Base Address, in 16-byte units, tmultibaseaddr==0 or
                                  //      LODBI==0 
                                  //      BaseAddress is the only valid section for BaseAddress 1, 2 and 3_8

        TileStride = 0xfe000000 // 31:25 texstride Tile stride (0 to 127 tiles).
    };

    namespace Shift
    {
        enum
        {
            MemoryType = 0x00,
            BaseAddress = 0x04,
            TileStride = 0x19
        };
    }
}

}