//Copyright 2023-Present riplin

#pragma once

#include <has/types.h>
#include <has/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>

namespace Hag::VGA
{

namespace Register
{
    
enum
{
    DACStatus = 0x3C7,                                      //DAC_STS
};

}

typedef uint8_t DACStatus_t;

namespace DACStatus
{
    
    enum //Mask
    {
        Status = 0x03   //The last executing cycle was:
                        //00 = Write Palette cycle
                        //11 = Read Palette cycle
                        //Reads from the RAMDAC Write Index at address 3C8H or
                        //the DAC status register at address 3C7H do not interfere
                        //with read or write cycles and may take place at any time.
    };

    namespace Shift
    {
        enum
        {
            Status = 0x00
        };
    }

    inline DACStatus_t Read()
    {
        return DACStatus_t(SYS_ReadPortByte(Register::DACStatus));
    }

}

}
