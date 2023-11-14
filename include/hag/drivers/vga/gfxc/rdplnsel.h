//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/data.h>

namespace Hag { namespace VGA { namespace GraphicsController
{

typedef uint8_t ReadPlaneSelect_t;

//The contents of this register represent the memory plane from which the CPU reads data in read mode O.
//This register has no effect on the color compare read mode (read mode 1). In odd/even mode, bit 0 is ignored.

namespace ReadPlaneSelect
{
    enum //Mask
    {
        ReadPlane = 0x03    //The memory plane is selected as follows:
                            //00 = Plane 0
                            //01 = Plane 1
                            //10 = Plane 2
                            //11 = Plane 3
    };

    namespace Shift
    {
        enum
        {
            ReadPlane = 0x00
        };
    }

    inline ReadPlaneSelect_t Read()
    {
        GraphicsControllerIndex::Write(GraphicsControllerRegister::ReadPlaneSelect);
        return ReadPlaneSelect_t(GraphicsControllerData::Read());
    }
    
    inline void Write(ReadPlaneSelect_t value)
    {
        GraphicsControllerData::Write(GraphicsControllerRegister::ReadPlaneSelect, GraphicsControllerData_t(value));
    }

}

}}}
