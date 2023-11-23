//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t CurrentYPosition2_t;

namespace CurrentYPosition2
{

    inline CurrentYPosition2_t Read()
    {
        return CurrentYPosition2_t(SYS_ReadPortShort(Register::CurrentYPosition2));
    }

    inline void Write(CurrentYPosition2_t value)
    {
        SYS_WritePortShort(Register::CurrentYPosition2, value);
    }
}

}}
