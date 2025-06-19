//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/bitplnrm.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        BitplaneReadMask = 0x812C,
    };
}

namespace MMIO::Packed::BitplaneReadMask
{
    inline BitplaneReadMask_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::BitplaneReadMask).ToPointer<BitplaneReadMask_t>();
    }

    inline void Write(BitplaneReadMask_t readMask)
    {
            Get() = readMask;
    }
}

}
