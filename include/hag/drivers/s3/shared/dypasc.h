//Copyright 2023-Present riplin

#pragma once

#include <has/farptr.h>
#include <has/system/sysasm.h>
#include <hag/drivers/s3/shared/regtype.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    DestinationYPositionAxialStepConstant = 0x8AE8,         //DESTY_AXSTP
};

}

typedef uint16_t DestinationYPositionAxialStepConstant_t;

namespace DestinationYPositionAxialStepConstant
{

    inline DestinationYPositionAxialStepConstant_t Read()
    {
        return DestinationYPositionAxialStepConstant_t(SYS_ReadPortShort(Register::DestinationYPositionAxialStepConstant));
    }

    inline void Write(DestinationYPositionAxialStepConstant_t value)
    {
        SYS_WritePortShort(Register::DestinationYPositionAxialStepConstant, value);
    }
}

namespace MMIO
{

    namespace DestinationYPositionAxialStepConstant
    {
        inline DestinationYPositionAxialStepConstant_t Read()
        {
            return *FARPointer(0xA000, Register::DestinationYPositionAxialStepConstant).
                ToPointer<DestinationYPositionAxialStepConstant_t>(sizeof(DestinationYPositionAxialStepConstant_t));
        }

        inline void Write(DestinationYPositionAxialStepConstant_t value)
        {
            *FARPointer(0xA000, Register::DestinationYPositionAxialStepConstant).
                ToPointer<DestinationYPositionAxialStepConstant_t>(sizeof(DestinationYPositionAxialStepConstant_t)) = value;
        }
    }

}

}
