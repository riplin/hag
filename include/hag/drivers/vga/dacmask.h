//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    DACMask = 0x3C6,                                        //DAC_AD_MK
};

}

//This register is the pixel read mask register to select pixel video output. The CPU can access this register at any time.
//The contents of this register are bit-wise logically ANDed
//with the pixel select video output (PA[7:0]). This register
//is initialized to FFH by the BIOS during a video mode set.

typedef uint8_t DACMask_t;

namespace DACMask
{
    
    inline DACMask_t Read()
    {
        return DACMask_t(SYS_ReadPortByte(Register::DACMask));
    }
    
    inline void Write(DACMask_t value)
    {
        SYS_WritePortByte(Register::DACMask, value);
    }

}

}}
