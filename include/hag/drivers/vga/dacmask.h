//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>


namespace Hag { namespace VGA
{

//This register is the pixel read mask register to select pixel video output. The CPU can access this register at any time.

typedef uint8_t DACMask_t;

namespace DACMask
{
    
    enum //Mask
    {
        Mask = 0xFF //The contents of this register are bit-wise logically ANDed
                    //with the pixel select video output (PA[7:0]). This register
                    //is initialized to FFH by the BIOS during a video mode set.
    };

    namespace Shift
    {
        enum
        {
            Mask = 0x00
        };
    }

    inline DACMask_t Read()
    {
        return DACMask_t(SYS_ReadPortByte(Register::DACMask));
    }
    
    inline void Write(DACMask_t value)
    {
        SYS_WritePortByte(Register::DACMask, value);
    }

    inline uint8_t GetMask(DACMask_t value)
    {
        return (value & Mask) >> Shift::Mask;
    }

    inline DACMask_t SetMask(uint8_t value)
    {
        return DACMask_t((value << Shift::Mask) & Mask);
    }

}

}}