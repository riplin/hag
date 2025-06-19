//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/regtype.h>
#include <hag/drivers/vga/gfxc/data.h>

namespace Hag::VGA::GraphicsController
{

namespace Register
{

enum
{
    EnableSetResetData = 0x01,              //EN_S/R_DT GR1
};

}

typedef uint8_t EnableSetResetData_t;

//These bits enable the set/reset data, and affect write mode O.

namespace EnableSetResetData
{
    enum //Mask
    {
        EnableSetReset = 0x0F   //When each bit is a logical 1, the respective memory plane
                                //is written with the value of the Set/Reset Data register.
                                //A logical 0 disables the set/reset data in a plane,
                                //and that plane is written with the value of CPU write data.
    };

    namespace Shift
    {
        enum
        {
            EnableSetReset = 0x00
        };
    }

    inline EnableSetResetData_t Read()
    {
        GraphicsControllerIndex::Write(Register::EnableSetResetData);
        return EnableSetResetData_t(GraphicsControllerData::Read());
    }
    
    inline void Write(EnableSetResetData_t value)
    {
        GraphicsControllerData::Write(Register::EnableSetResetData, GraphicsControllerData_t(value));
    }

}

}
