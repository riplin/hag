//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    DestinationXPositionDiagonalStepConstant = 0x8EE8,      //DESTX_DIASTP
};

}

typedef uint16_t DestinationXPositionDiagonalStepConstant_t;

namespace DestinationXPositionDiagonalStepConstant
{

    inline DestinationXPositionDiagonalStepConstant_t Read()
    {
        return DestinationXPositionDiagonalStepConstant_t(SYS_ReadPortShort(Register::DestinationXPositionDiagonalStepConstant));
    }

    inline void Write(DestinationXPositionDiagonalStepConstant_t value)
    {
        SYS_WritePortShort(Register::DestinationXPositionDiagonalStepConstant, value);
    }
}

namespace MMIO
{

    namespace DestinationXPositionDiagonalStepConstant
    {
        inline DestinationXPositionDiagonalStepConstant_t Read()
        {
            return *FARPointer(0xA000, Register::DestinationXPositionDiagonalStepConstant).
                ToPointer<DestinationXPositionDiagonalStepConstant_t>(sizeof(DestinationXPositionDiagonalStepConstant_t));
        }

        inline void Write(DestinationXPositionDiagonalStepConstant_t value)
        {
            *FARPointer(0xA000, Register::DestinationXPositionDiagonalStepConstant).
                ToPointer<DestinationXPositionDiagonalStepConstant_t>(sizeof(DestinationXPositionDiagonalStepConstant_t)) = value;
        }
    }

}

}
