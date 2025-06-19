//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    ColorCompare = 0xB2E8,                                  //COLOR_CMP
};

}

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

namespace MMIO
{

    namespace ColorCompare
    {
        inline ColorCompare_t Read()
        {
            ColorCompare_t lower = *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t));
            BitplaneReadMask_t upper = *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t));
            return (upper << 16) | lower;
        }

        inline void Write(ColorCompare_t value)
        {
            *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::ColorCompare).
                ToPointer<ColorCompare_t>(sizeof(ColorCompare_t)) = uint16_t(value >> 16);
        }
    }

}

}
