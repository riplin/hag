//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t CurrentXPosition_t;

namespace CurrentXPosition
{

    inline CurrentXPosition_t Read()
    {
        return CurrentXPosition_t(SYS_ReadPortShort(Register::CurrentXPosition));
    }

    inline void Write(CurrentXPosition_t value)
    {
        SYS_WritePortShort(Register::CurrentXPosition, value);
    }
}

}}
