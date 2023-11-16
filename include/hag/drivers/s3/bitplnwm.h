//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint32_t BitplaneWriteMask_t;

//If bit i = 0, bitplane i is not updated If bit i = 1, bitplane i is updated
//Bits 31-0 control planes 31-0 respectively. If bit 9 of BEE8_EH is set to 1 for programmed I/O (not MMIO),
//this becomes a 32-bit register. If bit 9 of BEE8_EH is cleared to 0, this is two 16-bit registers.
//In 32 bpp mode with 16-bit registers, the upper and lower doublewords are read or written sequentially,
//depending on the state of the RSF flag (bit 4 of BEE8H, Index EH). If RSF = 0, the lower 16 bits are accessed.
//If RSF = 1, the upper 16 bits are accessed. The RSF flag toggles automatically when a double-word is read or written.

namespace BitplaneWriteMask
{
    enum
    {
        NoneSet = 0x00000000,
        AllSet = 0xFFFFFFFF
    };

    inline BitplaneWriteMask_t Read16x2()
    {
        return BitplaneWriteMask_t(SYS_ReadPortShort(Register::BitplaneWriteMask)) |
               (BitplaneWriteMask_t(SYS_ReadPortShort(Register::BitplaneWriteMask)) << 16);
    }

    inline BitplaneWriteMask_t Read32x1()
    {
        return BitplaneWriteMask_t(SYS_ReadPortDouble(Register::BitplaneWriteMask));
    }

    inline void Write16x2(BitplaneWriteMask_t value)
    {
        SYS_WritePortShort(Register::BitplaneWriteMask, uint16_t(value));
        SYS_WritePortShort(Register::BitplaneWriteMask, uint16_t(value >> 16));
    }

    inline void Write32x1(BitplaneWriteMask_t value)
    {
        SYS_WritePortDouble(Register::BitplaneWriteMask, value);
    }
}

}}
