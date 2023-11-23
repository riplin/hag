//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint32_t ColorCompare_t;

namespace ColorCompare
{

    inline ColorCompare_t Read16x2()
    {
        return ColorCompare_t(SYS_ReadPortShort(Register::ColorCompare)) |
               (ColorCompare_t(SYS_ReadPortShort(Register::ColorCompare)) << 16);
    }

    inline ColorCompare_t Read32x1()
    {
        return ColorCompare_t(SYS_ReadPortDouble(Register::ColorCompare));
    }

    inline void Write16x2(ColorCompare_t value)
    {
        SYS_WritePortShort(Register::ColorCompare, uint16_t(value));
        SYS_WritePortShort(Register::ColorCompare, uint16_t(value >> 16));
    }

    inline void Write32x1(ColorCompare_t value)
    {
        SYS_WritePortDouble(Register::ColorCompare, value);
    }

}

}}
