//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/curxpos.h>
#include <hag/drivers/s3/shared/curypos.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        CurrentXYPosition = 0x8100,
    };
}}

namespace MMIO { namespace Packed { namespace CurrentXYPosition
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::CurrentXYPosition).ToPointer<uint32_t>();
    }

    inline void Write(CurrentXPosition_t xPosition, CurrentYPosition_t yPosition)
    {
            Get() = (uint32_t(xPosition) << 16) | yPosition;
    }
}}}

}}}