//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/instat0.h>

//Extend the Input Status 0 register with additional bits.

namespace Hag { namespace VGA
{

namespace InputStatus0
{

    enum //Mask
    {
        CRTInterruptStatus = 0x40       //CRT INTPE 0 = Vertical retrace interrupt cleared, 1 = Vertical retrace interrupt pending
    };

    namespace Shift
    {
        enum
        {
            CRTInterruptStatus = 0x06
        };
    };

}

}}
