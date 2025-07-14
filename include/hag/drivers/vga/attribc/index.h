//Copyright 2023-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/vga/attribc/regtype.h>

namespace Hag::VGA
{

namespace Register
{
    
enum
{
    AttributeControllerIndex = 0x3C0,                       //ATR_AD
};

}

//How to use:
//http://www.osdever.net/FreeVGA/vga/vgareg.htm#attribute

//This register is loaded with a binary index value that determines which attribute controller 
//register will be accessed. This value is referred to as the "Index Number" of the AR register (ARO-14).

namespace AttributeControllerIndex
{
    enum //Mask
    {
        AttributeControllerIndexIndex = 0x1F,   //A binary value that points to the attribute controller
                                                //register where data is to be written.
        EnableVideoDisplay = 0x20               //0 = Video display access to the palette registers disabled.
                                                //    The Attribute Controller register can be accessed by the CPU.
                                                //1 = Display video using the palette registers enabled (normal 
                                                //    display operation). The palette registers (ARO-ARF) cannot
                                                //    be accessed by the CPU This bit is effective only in 8-bit
                                                //    PA mode (CR67_4= 0).
    };

    namespace Shift
    {
        enum
        {
            AttributeControllerIndexIndex = 0x00,
            EnableVideoDisplay = 0x05
        };
    }

    inline AttributeController::Register_t Read()
    {
        return AttributeController::Register_t(SYS_ReadPortByte(Register::AttributeControllerIndex));
    }
    
    inline void Write(AttributeController::Register_t value)
    {
        SYS_WritePortByte(Register::AttributeControllerIndex, value);
    }

    inline void ResetIndex(VGA::Register_t inputStatus1)
    {
        //Pulling Input Status 1 resets the Attribute Index.
        InputStatus1::Read(inputStatus1);
    }

}

}
