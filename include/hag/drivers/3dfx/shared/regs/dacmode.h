//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        DACMode = 0x4c
    };
}

typedef uint32_t DACMode_t;
namespace DACMode
{
    enum //Mask
    {
        Mode = 0x0001,
        OneToOne = 0x0000,
        TwoToOne = 0x0001,
        VSyncDPMS = 0x0002,
        VSyncDPMSDisable = 0x0000,
        VSyncDPMSEnable = 0x0002,
        VSyncValue = 0x0004,
        VSyncOff = 0x0000,
        VSyncOn = 0x0004,
        HSyncDPMS = 0x0008,
        HSyncDPMSDisable = 0x0000,
        HSyncDPMSEnable = 0x0008,
        HSyncValue = 0x0010,
        HSyncOff = 0x0000,
        HSyncOn = 0x0010,
        CRC2CollectionMode = 0x0020
    };

    namespace Shift
    {
        enum
        {
            Mode = 0x00,
            VSyncDPMS = 0x01,
            VSyncValue = 0x02,
            HSyncDPMS = 0x03,
            HSyncValue = 0x04,
            CRC2CollectionMode = 0x05
        };
    }
}

}