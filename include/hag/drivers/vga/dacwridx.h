//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>


namespace Hag { namespace VGA
{

//This register contains the pointer to one of 256 palette
//data registers and is used dur- ing a palette load. Each
//time the color code is written to this register, it
//identifies that a write sequence will occur. A write
//sequence consists of three successive byte writes to the
//DAC data register at I/O address 3C9H. The least
//significant 6 bits of each byte are concatenated to form
//the value placed in the 18-bit data register. The order
//is red byte first, then green, and finally blue. Once the
//third byte has been written, the value in the data
//register is written to the location pointed to by the
//color code. The se- quence of events for a write cycle is:
//1. Write the color code to this register (DAC Write Index)
//   at address 3C8H.
//2. Three bytes are written to the DAC Data register at
//   address 3C9H.
//3. The contents of the DAC data register are transferred
//   to the location in the color look-up table pointed to
//   by the color code.
//4. The DAC Write Index register auto-increments by 1.
//5. Goto step 2.

typedef uint8_t DACWriteIndex_t;

namespace DACWriteIndex
{
    
    inline DACWriteIndex_t Read()
    {
        return DACWriteIndex_t(SYS_ReadPortByte(Register::DACWriteIndex));
    }
    
    inline void Write(DACWriteIndex_t value)
    {
        SYS_WritePortByte(Register::DACWriteIndex, value);
    }

}

}}
