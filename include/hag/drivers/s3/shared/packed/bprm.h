//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/bitplnrm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        BitplaneReadMask = 0x812C,
    };
}}

namespace MMIO { namespace Packed { namespace BitplaneReadMask
{
    inline BitplaneReadMask_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::BitplaneReadMask).ToPointer<BitplaneReadMask_t>();
    }

    inline void Write(BitplaneReadMask_t readMask)
    {
            Get() = readMask;
    }
}}}

}}}
