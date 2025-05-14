//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/regtype.h>
#include <hag/drivers/vga/miscout.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{
    
enum
{
    MiscellaneousW = VGA::Register::MiscellaneousW,
    MiscellaneousR = VGA::Register::MiscellaneousR
};

}

typedef VGA::MiscellaneousOutput_t MiscellaneousOutput_t;

namespace MiscellaneousOutput
{
    using namespace VGA::MiscellaneousOutput;

    enum //Mask
    {
        ClockSelectDCLKPLL = 0x0C                           //11 = Enables loading of DCLK PLL parameters in SR12 and SR13.
    };

    namespace Shift
    {
        using namespace VGA::MiscellaneousOutput::Shift;
    }
}

}}}
