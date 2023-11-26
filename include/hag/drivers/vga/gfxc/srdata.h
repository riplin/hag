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
    SetResetData = 0x00,                    //SET/RST_DT GR0
};

}

typedef uint8_t SetResetData_t;

//This register represents the value written to all 8 bits of the respective memory plane when
//the CPU executes a memory write in write modes 0 and 3.

namespace SetResetData
{
    enum //Mask
    {
        SetReset = 0x0F         //These bits become the color value for CPU memory write operations.
                                //In write mode 0, the set/reset data can be enabled on the corresponding bit of the 
                                //Enable Set/Reset Data register. In write mode 3, there is no effect on the
                                //Set/Reset Data register.
    };

    namespace Shift
    {
        enum
        {
            SetReset = 0x00
        };
    }

    inline SetResetData_t Read()
    {
        GraphicsControllerIndex::Write(Register::SetResetData);
        return SetResetData_t(GraphicsControllerData::Read());
    }
    
    inline void Write(SetResetData_t value)
    {
        GraphicsControllerData::Write(Register::SetResetData, GraphicsControllerData_t(value));
    }

}

}}}
