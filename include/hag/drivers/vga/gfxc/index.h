//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace VGA
{

typedef uint8_t GraphicsControllerIndex_t;

namespace GraphicsControllerIndex
{
    enum //Mask
    {
        GraphicsControllerRegisterIndex = 0x0F   //GRC ADDRESS
    };

    namespace Shift
    {
        enum
        {
            GraphicsControllerRegisterIndex = 0x00
        };
    }

    inline GraphicsControllerIndex_t Read()
    {
        return GraphicsControllerIndex_t(SYS_ReadPortByte(Register::GraphicsControllerIndex));
    }
    
    inline void Write(GraphicsControllerIndex_t value)
    {
        SYS_WritePortByte(Register::GraphicsControllerIndex, value);
    }

}

}}
