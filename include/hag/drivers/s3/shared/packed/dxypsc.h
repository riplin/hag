//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/dxpdsc.h>
#include <hag/drivers/s3/shared/dypasc.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        DestinationXYPositionStepConstant = 0x8108,
    };
}}

namespace MMIO { namespace Packed { namespace DestinationXYPositionStepConstant
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::DestinationXYPositionStepConstant).ToPointer<uint32_t>();
    }

    inline void Write(DestinationXPositionDiagonalStepConstant_t destinationX, DestinationYPositionAxialStepConstant_t destinationY)
    {
            Get() = (uint32_t(destinationX) << 16) | destinationY;
    }
}}}

}}}
