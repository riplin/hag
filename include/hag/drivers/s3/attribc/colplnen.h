//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/colplnen.h>

//Extend the VGA Color Plane register with additional bits.

namespace Hag { namespace VGA { namespace AttributeController
{

namespace ColorPlane
{

    enum //Mask
    {
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
            VideoTestSelect = 0x04
        };
    }
    
}

}}}
