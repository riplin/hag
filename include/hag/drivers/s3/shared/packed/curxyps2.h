//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/curxpos2.h>
#include <hag/drivers/s3/shared/curypos2.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        CurrentXYPosition2 = 0x8104,
    };
}}

namespace MMIO { namespace Packed { namespace CurrentXYPosition2
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::CurrentXYPosition2).ToPointer<uint32_t>();
    }

    inline void Write(CurrentXPosition2_t xPosition2, CurrentYPosition2_t yPosition2)
    {
            Get() = (uint32_t(xPosition2) << 16) | yPosition2;
    }
}}}

}}}
