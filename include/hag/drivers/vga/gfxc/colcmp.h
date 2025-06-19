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
    ColorCompare = 0x02,                    //COLOR-CMP GR2
};

}

typedef uint8_t ColorCompare_t;

//These bits represent a 4-bit color value to be compared. In read mode 1, the CPU executes a memory read, 
//the read data is compared with this value and returns the results. This register works in conjunction with
//the Color Don't Care register.


namespace ColorCompare
{
    enum //Mask
    {
        ColorCompareData = 0x0F //This value becomes the reference color used to compare each pixel.
                                //Each of the 8-bit positions of the read data are compared across
                                //four planes and a logical 1 is returned in each bit position for
                                //which the colors match.

    };

    namespace Shift
    {
        enum
        {
            ColorCompareData = 0x00
        };
    }

    inline ColorCompare_t Read()
    {
        GraphicsControllerIndex::Write(Register::ColorCompare);
        return ColorCompare_t(GraphicsControllerData::Read());
    }
    
    inline void Write(ColorCompare_t value)
    {
        GraphicsControllerData::Write(Register::ColorCompare, GraphicsControllerData_t(value));
    }

}

}
