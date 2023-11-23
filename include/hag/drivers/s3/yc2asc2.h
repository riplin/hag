//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t YCoordinate2AxialStepConstant2_t;

namespace YCoordinate2AxialStepConstant2
{

    inline YCoordinate2AxialStepConstant2_t Read()
    {
        return YCoordinate2AxialStepConstant2_t(SYS_ReadPortShort(Register::YCoordinate2AxialStepConstant2));
    }

    inline void Write(YCoordinate2AxialStepConstant2_t value)
    {
        SYS_WritePortShort(Register::YCoordinate2AxialStepConstant2, value);
    }
}

}}
