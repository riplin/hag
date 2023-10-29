//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/index.h>
#include <hag/drivers/vga/gfxc/regs.h>

namespace Hag { namespace VGA
{

typedef uint8_t GraphicsControllerData_t;

namespace GraphicsControllerData
{
    inline GraphicsControllerData_t Read()
    {
        return GraphicsControllerData_t(SYS_ReadPortByte(Register::GraphicsControllerData));
    }

    inline void Write(GraphicsControllerData_t value)
    {
        SYS_WritePortByte(Register::GraphicsControllerData, value);
    }

    inline void Write(GraphicsControllerIndex_t gfxIndex, GraphicsControllerData_t value)
    {
        SYS_WritePortBytes(Register::GraphicsControllerIndex, gfxIndex, value);
    }
}

}}
