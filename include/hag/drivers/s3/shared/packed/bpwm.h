//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/bitplnwm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        BitplaneWriteMask = 0x8128,
    };
}}

namespace MMIO { namespace Packed { namespace BitplaneWriteMask
{
    inline BitplaneWriteMask_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::BitplaneWriteMask).ToPointer<BitplaneWriteMask_t>();
    }

    inline void Write(BitplaneWriteMask_t writeMask)
    {
            Get() = writeMask;
    }
}}}

}}}
