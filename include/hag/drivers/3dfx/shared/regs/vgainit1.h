//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        VGAInit1 = 0x2c
    };
}

typedef uint32_t VGAInit1_t;
namespace VGAInit1
{
    enum //Mask
    {
        VBEWriteAperture32K         = 0x000003ff,    // VBE write Aperture, in 32K granularity 
        VBEReadAperture32K          = 0x000ffc00,    // VBE read Aperture, in 32K granularity 
        EnableChain4Mode            = 0x00100000,    // Enable 0xA0000 Sequential Chain 4 mode. 
        LockHorizontalTiming        = 0x00200000,    // Lock Horizontal Timing - 3B4/3D4 index 0,1,2,3,4,5,1a
        LockVerticalTiming          = 0x00400000,    // Lock Vertical Timing -3B4/3D4 index 6,7 (bit 7,5,3,2 and 0), 9 10, 11 (bits[3:0]), 15,16,1b.
        LockH2                      = 0x00800000,    // Lock H2 - 0x3B4/0x3D4 index 17, bit 2
        LockVSync                   = 0x01000000,    // Lock Vsync - 0x3C2, bit 7.
        LockHSync                   = 0x02000000,    // Lock HSync - 0x3C2, bit 6.
        LockClockSelect             = 0x04000000,    // Lock Clock Select - 0x3C2, bits 3 and 2.
        LockRamEnable               = 0x08000000,    // Lock Ram Enable - 0x3C2, bit 1.
        LockCharacterClock          = 0x10000000     // Lock Character Clock - 0x3C4, index 1 bit 0.
    };

    namespace Shift
    {
        enum
        {
            VBEWriteAperture32K         = 0x00,
            VBEReadAperture32K          = 0x0a,
            EnableChain4Mode            = 0x14,
            LockHorizontalTiming        = 0x15,
            LockVerticalTiming          = 0x16,
            LockH2                      = 0x17,
            LockVSync                   = 0x18,
            LockHSync                   = 0x19,
            LockClockSelect             = 0x1a,
            LockRamEnable               = 0x1b,
            LockCharacterClock          = 0x1c
        };
    }
}

}