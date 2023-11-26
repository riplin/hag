//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/fgcolor.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        ForegroundColor = 0x8124,
    };
}}

namespace MMIO { namespace Packed { namespace ForegroundColor
{
    inline ForegroundColor_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ForegroundColor).ToPointer<ForegroundColor_t>();
    }

    inline void Write(ForegroundColor_t foregroundColor)
    {
            Get() = foregroundColor;
    }
}}}

}}}
