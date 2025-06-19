//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/bgcolor.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        BackgroundColor = 0x8120,
    };
}

namespace MMIO::Packed::BackgroundColor
{
    inline BackgroundColor_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::BackgroundColor).ToPointer<BackgroundColor_t>();
    }

    inline void Write(BackgroundColor_t backgroundColor)
    {
            Get() = backgroundColor;
    }
}

}
