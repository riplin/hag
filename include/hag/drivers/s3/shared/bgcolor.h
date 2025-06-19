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
    BackgroundColor = 0xA2E8,                               //BKGD_COLOR
};

}

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

namespace MMIO
{

    namespace BackgroundColor
    {
        inline BackgroundColor_t Read()
        {
            BackgroundColor_t lower = *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t));
            BackgroundColor_t upper = *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t));
            return (upper << 16) | lower;
        }

        inline void Write(BackgroundColor_t value)
        {
            *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::BackgroundColor).
                ToPointer<BackgroundColor_t>(sizeof(BackgroundColor_t)) = uint16_t(value >> 16);
        }
    }

}

}
