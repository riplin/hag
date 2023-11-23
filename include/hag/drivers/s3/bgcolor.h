//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint32_t BackgroundColor_t;

namespace BackgroundColor
{

    inline BackgroundColor_t Read16x2()
    {
        return BackgroundColor_t(SYS_ReadPortShort(Register::BackgroundColor)) |
               (BackgroundColor_t(SYS_ReadPortShort(Register::BackgroundColor)) << 16);
    }

    inline BackgroundColor_t Read32x1()
    {
        return BackgroundColor_t(SYS_ReadPortDouble(Register::BackgroundColor));
    }

    inline void Write16x2(BackgroundColor_t value)
    {
        SYS_WritePortShort(Register::BackgroundColor, uint16_t(value));
        SYS_WritePortShort(Register::BackgroundColor, uint16_t(value >> 16));
    }

    inline void Write32x1(BackgroundColor_t value)
    {
        SYS_WritePortDouble(Register::BackgroundColor, value);
    }
}

}}
