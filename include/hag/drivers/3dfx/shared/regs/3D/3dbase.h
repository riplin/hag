//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        Base =                  0x00200000,
        Swizzle =               0x00100000,
        Wrap =                  0x000fc000,
        Chip =                  0x00003c00,
        ChipFbiTrex_0s =        0x00000000,
        ChipFbi =               0x00000400,
        ChipTrex0 =             0x00000800,
        ChipFbiTrex0 =          ChipFbi | ChipTrex0,
        ChipTrex1 =             0x00001000,
        ChipFbiTrex1 =          ChipFbi | ChipTrex1,
        ChipTrex0Trex1 =        ChipTrex0 | ChipTrex1,
        ChipFbiTrex0Trex1 =     ChipFbi | ChipTrex0 | ChipTrex1,
        ChipTrex2 =             0x00002000,
        ChipFbiTrex2 =          ChipFbi | ChipTrex2,
        ChipTrex0Trex2 =        ChipTrex0 | ChipTrex2,
        ChipFbiTrex0Trex2 =     ChipFbi | ChipTrex0 | ChipTrex2,
        ChipTrex1Trex2 =        ChipTrex1 | ChipTrex2,
        ChipFbiTrex1Trex2 =     ChipFbi | ChipTrex1 | ChipTrex2,
        ChipTrex0Trex1Trex2 =   ChipTrex0 |ChipTrex1 | ChipTrex2,
        ChipFbiTrex_1s =        ChipFbi | ChipTrex0 | ChipTrex1 | ChipTrex2,
        Register =              0x000003fc
    };
}

}