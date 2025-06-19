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
    BitplaneReadMask = 0xAEE8,                              //RD_MASK
};

}

typedef uint32_t BitplaneReadMask_t;

namespace BitplaneReadMask
{
    enum
    {
        NoneSet = 0x00000000,
        AllSet = 0xFFFFFFFF
    };

    inline BitplaneReadMask_t Read16x2()
    {
        return BitplaneReadMask_t(SYS_ReadPortShort(Register::BitplaneReadMask)) |
               (BitplaneReadMask_t(SYS_ReadPortShort(Register::BitplaneReadMask)) << 16);
    }

    inline BitplaneReadMask_t Read32x1()
    {
        return BitplaneReadMask_t(SYS_ReadPortDouble(Register::BitplaneReadMask));
    }

    inline void Write16x2(BitplaneReadMask_t value)
    {
        SYS_WritePortShort(Register::BitplaneReadMask, uint16_t(value));
        SYS_WritePortShort(Register::BitplaneReadMask, uint16_t(value >> 16));
    }

    inline void Write32x1(BitplaneReadMask_t value)
    {
        SYS_WritePortDouble(Register::BitplaneReadMask, value);
    }
}

namespace MMIO
{

    namespace BitplaneReadMask
    {
        inline BitplaneReadMask_t Read()
        {
            BitplaneReadMask_t lower = *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t));
            BitplaneReadMask_t upper = *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t));
            return (upper << 16) | lower;
        }

        inline void Write(BitplaneReadMask_t value)
        {
            *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t)) = uint16_t(value);
            *FARPointer(0xA000, Register::BitplaneReadMask).
                ToPointer<BitplaneReadMask_t>(sizeof(BitplaneReadMask_t)) = uint16_t(value >> 16);
        }
    }

}

}
