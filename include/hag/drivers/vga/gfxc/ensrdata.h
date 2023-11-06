//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/data.h>

namespace Hag { namespace VGA { namespace GraphicsController
{

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
        GraphicsControllerIndex::Write(GraphicsControllerRegister::EnableSetResetData);
        return EnableSetResetData_t(GraphicsControllerData::Read());
    }
    
    inline void Write(EnableSetResetData_t value)
    {
        GraphicsControllerData::Write(GraphicsControllerRegister::EnableSetResetData, GraphicsControllerData_t(value));
    }

    inline uint4_t GetEnableSetResetData(EnableSetResetData_t value)
    {
        return (value & EnableSetReset) >> Shift::EnableSetReset;
    }

    inline EnableSetResetData_t SetEnableSetResetData(uint4_t value)
    {
        return EnableSetResetData_t(value << Shift::EnableSetReset) & EnableSetReset;
    }
}

}}}