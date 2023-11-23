//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t CurrentXPosition2_t;

namespace CurrentXPosition2
{

    inline CurrentXPosition2_t Read()
    {
        return CurrentXPosition2_t(SYS_ReadPortShort(Register::CurrentXPosition2));
    }

    inline void Write(CurrentXPosition2_t value)
    {
        SYS_WritePortShort(Register::CurrentXPosition2, value);
    }
}

}}
