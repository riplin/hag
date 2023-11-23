//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

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

}}
