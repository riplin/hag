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
        DisplayPlaneEnable = 0x0F,  //A 0 in any of these bits forces the corresponding color plane 
                                    //bit to 0 before accessing the internal palette. A 1 in any of
                                    //these bits enables the data on the corresponding color plane.
        VideoTestSelect = 0x30      //These bits select two of the eight bit color outputs to be
                                    //available in the Input Status 1 register. The output color
                                    //combinations available on the status bits are as follows:
                                    //      D STS MUX              STS1
                                    //Bit 5     Bit 4       Bit 5       Bit 4
                                    //  0         0        Video 2     Video 0
                                    //  0         1        Video 5     Video 4
                                    //  1         0        Video 3     Video 1
                                    //  1         1        Video 7     Video 6
    };

    namespace Shift
    {
        enum
        {
            DisplayPlaneEnable = 0x00,
            VideoTestSelect = 0x04
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
