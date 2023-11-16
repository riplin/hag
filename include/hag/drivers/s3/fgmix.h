//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t ForegroundMix_t;

namespace ForegroundMix
{
    enum
    {
        MixTypeMask = 0x000F,
        //In the general case, a new color is defined. A logical operation such as AND or OR is then performed
        //between it and the current bitmap color. If the bitplane to be written is enabled, the result of this
        //logical "mix" is written to the bitmap as the new pixel color. 
        //The following table shows the mix types available (! = logical NOT).
        //0000 !current                 1000 !current OR !new
        //0001 logical zero             1001 current OR !new
        //0010 logical one              1010 !current OR new
        //0011 leave current as-is      1011 current OR new
        //0100 !new                     1100 current AND new
        //0101 current XOR new          1101 !current AND new
        //0110 !(current XOR new)       1110 current AND !new
        //0111 new                      1111 !current AND !new
        MixNotCurrent = 0x0000,
        MixLogicalZero = 0x0001,
        MixLogicalOne = 0x0002,
        MixLeaveAsIs = 0x0003,
        MixNotNew = 0x0004,
        MixCurrentXorNew = 0x0005,
        MixCurrentXorNewNot = 0x0006,
        MixNew = 0x0007,
        MixNotCurrentOrNotNew = 0x0008,
        MixCurrentOrNotNew = 0x0009,
        MixNotCurrentOrNew = 0x000A,
        MixCurrentOrNew = 0x000B,
        MixCurrentAndNew = 0x000C,
        MixNotCurrentAndNew = 0x000D,
        MixCurrentAndNotNew = 0x000E,
        MixNotCurrentAndNotNew = 0x000F,
        SelectColorSourceMask = 0x0060,
        SelectBackgroundColor = 0x0000, //The register is the color source
        SelectForegroundColor = 0x0020, //The register is the color source
        SelectCPUData = 0x0040,         //The CPU is the color source
        SelectDisplayMemory = 0x0060,   //The display memory is the color source
    };

    inline ForegroundMix_t Read()
    {
        return ForegroundMix_t(SYS_ReadPortShort(Register::ForegroundMix));
    }

    inline void Write(ForegroundMix_t value)
    {
        SYS_WritePortShort(Register::ForegroundMix, value);
    }
}

}}
