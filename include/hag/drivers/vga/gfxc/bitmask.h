//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/regtype.h>
#include <hag/drivers/vga/gfxc/data.h>

namespace Hag { namespace VGA { namespace GraphicsController
{

namespace Register
{

enum
{
    BitMask = 0x08                          //BIT_MASK GR8
};

}

typedef uint8_t BitMask_t;

//Any bit programmed to 0 in this register will cause the corresponding bit in each of four
//memory planes to be immune to change. The data written into memory in this case is the data
//which was read in the previous cycle, and was stored in the processor latches. Any bit
//programmed to 1 allows unimpeded writes to the corresponding bits in the plane.

namespace BitMask
{
    enum //Mask
    {
        Mask = 0xFF //A logical 0 means the corresponding bit of each plane in memory is set
                    //to the corre- sponding bit in the processor latches. A logical 1 means
                    //the corresponding bit of each plane in memory is set as specified
                    //by other conditions.
    };

    namespace Shift
    {
        enum
        {
            Mask = 0x00
        };
    }

    inline BitMask_t Read()
    {
        GraphicsControllerIndex::Write(Register::BitMask);
        return BitMask_t(GraphicsControllerData::Read());
    }
    
    inline void Write(BitMask_t value)
    {
        GraphicsControllerData::Write(Register::BitMask, GraphicsControllerData_t(value));
    }

}

}}}
