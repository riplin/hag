//Copyright 2023-Present riplin

#pragma once

#include <hag/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/vga/gfxc/regtype.h>

namespace Hag::VGA
{

namespace Register
{
    
enum
{
    GraphicsControllerIndex = 0x3CE,                        //GRC_ADR
};

}

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

    inline GraphicsController::Register_t Read()
    {
        return GraphicsController::Register_t(SYS_ReadPortByte(Register::GraphicsControllerIndex));
    }
    
    inline void Write(GraphicsController::Register_t value)
    {
        SYS_WritePortByte(Register::GraphicsControllerIndex, value);
    }

}

}
