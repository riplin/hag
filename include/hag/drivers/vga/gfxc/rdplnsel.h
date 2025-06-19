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
    ReadPlaneSelect = 0x04,                 //RD_PL_SL GR4
};

}

typedef uint8_t ReadPlaneSelect_t;

//The contents of this register represent the memory plane from which the CPU reads data in read mode O.
//This register has no effect on the color compare read mode (read mode 1). In odd/even mode, bit 0 is ignored.

namespace ReadPlaneSelect
{
    enum //Mask
    {
        ReadPlane = 0x03,   //The memory plane is selected as follows:
        Plane1 = 0x00,      //00 = Plane 1
        Plane2 = 0x01,      //01 = Plane 2
        Plane3 = 0x02,      //10 = Plane 3
        Plane4 = 0x03,      //11 = Plane 4
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
        GraphicsControllerIndex::Write(Register::ReadPlaneSelect);
        return ReadPlaneSelect_t(GraphicsControllerData::Read());
    }
    
    inline void Write(ReadPlaneSelect_t value)
    {
        GraphicsControllerData::Write(Register::ReadPlaneSelect, GraphicsControllerData_t(value));
    }

}

}
