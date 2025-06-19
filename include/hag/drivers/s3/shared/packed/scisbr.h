//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        ScissorsBottomRight = 0x813C,
    };
}

namespace MMIO::Packed::ScissorsBottomRight
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ScissorsBottomRight).ToPointer<uint32_t>();
    }

    inline void Write(RightScissors_t rightScissors, BottomScissors_t bottomScissors)
    {
            Get() = (uint32_t(rightScissors) << 16) | bottomScissors;
    }
}

}

