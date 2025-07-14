//Copyright 2023-Present riplin

#pragma once

#include <has/types.h>
#include <has/system/sysasm.h>

namespace Hag::VGA
{

namespace Register
{
    
enum
{
    DACReadIndex = 0x3C7,                                   //DAC_RD_AD
};

}

//This register contains the pointer to one of 256 palette data registers and is used when reading the color palette.
//Each time the color code is written to this register, it identifies
//that a read sequence will occur. A read sequence consists of three
//successive byte reads from the RAM- DAC data register at I/O address
//3C9H. The least significant 6 bits of each byte taken from the RAMDAC
//data register contain the corresponding color value, and the most
//significant 2 bits contain zeros. The order is red byte first, then
//green, and finally blue. The sequence of events for a read cycle is:
//1. Write the color code to this register (RAMDAC Read Index) at address 3C7H.
//2. The contents of the location in the color look-up table pointed to by
//   the color code are transferred to the RAMDAC data register at address 3C9H.
//3. Three bytes are read back from the RAMDAC data register.
//4. The contents of this register auto-increment by one.
//5. Go to step 2.
//The effects of writing to the RAMDAC data register during a three-byte read
//cycle or reading from the RAMDAC data register during a 3-byte write cycle
//(ie., interrupting the sequence) are undefined and may change the look-up
//table contents.

typedef uint8_t DACReadIndex_t;

namespace DACReadIndex
{

    inline void Write(DACReadIndex_t value)
    {
        SYS_WritePortByte(Register::DACReadIndex, value);
    }

}

}
