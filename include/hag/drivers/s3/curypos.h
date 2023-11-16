//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t CurrentYPosition_t;

namespace CurrentYPosition
{

    inline CurrentYPosition_t Read()
    {
        return CurrentYPosition_t(SYS_ReadPortShort(Register::CurrentYPosition));
    }

    inline void Write(CurrentYPosition_t value)
    {
        SYS_WritePortShort(Register::CurrentYPosition, value);
    }
}

}}
