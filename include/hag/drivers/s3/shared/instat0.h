//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/instat0.h>

//Extend the Input Status 0 register with additional bits.

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{
    using namespace VGA::Register;
}

typedef Hag::VGA::InputStatus0_t InputStatus0_t;

namespace InputStatus0
{
    
    using namespace Hag::VGA::InputStatus0;

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

}}}
