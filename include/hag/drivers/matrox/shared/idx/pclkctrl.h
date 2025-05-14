//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{
    
enum
{
    PixelClockControl = 0x1A
};

}

typedef uint8_t PixelClockControl_t;

namespace PixelClockControl
{
    enum //Mask
    {
        ClockSelection = 0x03,          // Pixel clock selection. These bits select the source of the pixel clock:
        ClockPCI = 0x00,                // 00 = selects the output of the PCI clock
        ClockPLL = 0x01,                // 01 = selects the output of the pixel clock PLL
        ClockVDCLK = 0x02,              // 10 = selects external source (from the VDCLK pin)
        Clock = 0x04,                   // Pixel clock disable. This bit controls the pixel clock output:
        ClockEnable = 0x00,             // 0 = enable pixel clock oscillations.
        ClockDisable = 0x04,            // 1 = stop pixel clock oscillations.
        PLLPower = 0x08,                // Pixel PLL power down.
        PLLPowerDown = 0x00,            // 0 = power down
        PLLPowerUp = 0x08,              // 1 = power up
    };

    namespace Shift
    {
        enum
        {
            ClockSelection = 0x00,
            Clock = 0x02,
            PLLPower = 0x03,
        };
    }
}

}}}}