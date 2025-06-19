//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regtype.h>
#include <hag/drivers/vga/miscout.h>

namespace Hag::Matrox::Shared
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
        ClockSelectMGAPixelClock = 0x0C                     //11 = Selects the MGA pixel clock.
    };

    namespace Shift
    {
        using namespace VGA::MiscellaneousOutput::Shift;
    }
}

}