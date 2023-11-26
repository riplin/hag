//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        ScissorsTopLeft = 0x8138,
    };
}}

namespace MMIO { namespace Packed { namespace ScissorsTopLeft
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ScissorsTopLeft).ToPointer<uint32_t>();
    }

    inline void Write(LeftScissors_t leftScissors, TopScissors_t topScissors)
    {
            Get() = (uint32_t(leftScissors) << 16) | topScissors;
    }
}}}

}}}