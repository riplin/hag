//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint32_t ForegroundColor_t;

namespace ForegroundColor
{

    inline ForegroundColor_t Read16x2()
    {
        return ForegroundColor_t(SYS_ReadPortShort(Register::ForegroundColor)) |
               (ForegroundColor_t(SYS_ReadPortShort(Register::ForegroundColor)) << 16);
    }

    inline ForegroundColor_t Read32x1()
    {
        return ForegroundColor_t(SYS_ReadPortDouble(Register::ForegroundColor));
    }

    inline void Write16x2(ForegroundColor_t value)
    {
        SYS_WritePortShort(Register::ForegroundColor, uint16_t(value));
        SYS_WritePortShort(Register::ForegroundColor, uint16_t(value >> 16));
    }

    inline void Write32x1(ForegroundColor_t value)
    {
        SYS_WritePortDouble(Register::ForegroundColor, value);
    }
}

}}
