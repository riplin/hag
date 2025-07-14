//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::TDfx::Shared::ThreeD
{

typedef uint32_t Chip_t;
namespace Chip
{
    enum
    {
        FbiTrex_0s =        0x00000000,
        Fbi =               0x00000400,
        Trex0 =             0x00000800,
        FbiTrex0 =          Fbi | Trex0,
        Trex1 =             0x00001000,
        FbiTrex1 =          Fbi | Trex1,
        Trex0Trex1 =        Trex0 | Trex1,
        FbiTrex0Trex1 =     Fbi | Trex0 | Trex1,
        Trex2 =             0x00002000,
        FbiTrex2 =          Fbi | Trex2,
        Trex0Trex2 =        Trex0 | Trex2,
        FbiTrex0Trex2 =     Fbi | Trex0 | Trex2,
        Trex1Trex2 =        Trex1 | Trex2,
        FbiTrex1Trex2 =     Fbi | Trex1 | Trex2,
        Trex0Trex1Trex2 =   Trex0 | Trex1 | Trex2,
        FbiTrex_1s =        Fbi | Trex0 | Trex1 | Trex2
    };
}

namespace Register
{
    enum
    {
        Base =                  0x00200000,
        Swizzle =               0x00100000,
        Wrap =                  0x000fc000,
        Chip =                  0x00003c00,
        Register =              0x000003fc
    };
}

}