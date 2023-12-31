//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/regtype.h>
#include <hag/drivers/vga/attribc/data.h>

namespace Hag { namespace VGA { namespace AttributeController
{

namespace Register
{

enum
{
    ColorPlane = 0x12,                      //DISP_PLN AR12
};

}

typedef uint8_t ColorPlane_t;

namespace ColorPlane
{
    enum //Mask
    {
        DisplayPlaneEnable = 0x0F   //A 0 in any of these bits forces the corresponding color plane 
                                    //bit to 0 before accessing the internal palette. A 1 in any of
                                    //these bits enables the data on the corresponding color plane.
    };

    namespace Shift
    {
        enum
        {
            DisplayPlaneEnable = 0x00
        };
    }

    inline ColorPlane_t Read()
    {
        AttributeControllerIndex::Write(Register::ColorPlane);
        return ColorPlane_t(AttributeControllerData::Read());
    }
    
    inline void Write(ColorPlane_t value)
    {
        AttributeControllerData::Write(Register::ColorPlane, AttributeControllerData_t(value));
    }

}

}}}
