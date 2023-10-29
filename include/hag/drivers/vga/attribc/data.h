//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/index.h>
#include <hag/drivers/vga/attribc/regs.h>
#include <hag/drivers/vga/instat1.h>

namespace Hag { namespace VGA
{

//How to use:
//http://www.osdever.net/FreeVGA/vga/vgareg.htm#attribute

typedef uint8_t AttributeControllerData_t;

namespace AttributeControllerData
{
    inline AttributeControllerData_t Read()
    {
        return AttributeControllerData_t(SYS_ReadPortByte(Register::AttributeControllerDataR));
    }

    //Make sure that the index has been properly primed and that no interrupts occur!
    inline void Write(AttributeControllerIndex_t attrIndex, AttributeControllerData_t value)
    {
        SYS_WritePortByte(Register::AttributeControllerIndex, attrIndex);
        SYS_WritePortByte(Register::AttributeControllerDataW, value);
    }
}

}}
