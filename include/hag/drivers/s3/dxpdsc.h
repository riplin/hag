//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

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

}}
