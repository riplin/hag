//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/bitplnwm.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        BitplaneWriteMask = 0x8128,
    };
}

namespace MMIO::Packed::BitplaneWriteMask
{
    inline BitplaneWriteMask_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::BitplaneWriteMask).ToPointer<BitplaneWriteMask_t>();
    }

    inline void Write(BitplaneWriteMask_t writeMask)
    {
            Get() = writeMask;
    }
}

}
