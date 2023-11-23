//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

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

}}
