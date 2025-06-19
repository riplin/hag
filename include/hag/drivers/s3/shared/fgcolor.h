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
    ForegroundColor = 0xA6E8,                               //FRGD_COLOR
};

}

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

namespace MMIO
{

    namespace ForegroundColor
    {
        inline ForegroundColor_t Read()
        {
            ForegroundColor_t lower = *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t));
            ForegroundColor_t upper = *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t));
            return (upper << 16) | lower;
        }

        inline void Write(ForegroundColor_t value)
        {
            *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::ForegroundColor).
                ToPointer<ForegroundColor_t>(sizeof(ForegroundColor_t)) = uint16_t(value >> 16);
        }
    }

}

}
