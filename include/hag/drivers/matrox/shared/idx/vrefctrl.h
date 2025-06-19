//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    VoltageReferenceControl = 0x18
};

}

typedef uint8_t VoltageReferenceControl_t;

namespace VoltageReferenceControl
{
    enum //Mask
    {
        SysPLLVoltRefBlockPower = 0x01,     // System PLL voltage reference block power down.
        SysPLLRefBlockPowerDown = 0x00,     // 0 = Power Down
        SysPLLRefBlockPowerUp = 0x01,       // 1 = Power Up
        SysPLLVoltRef = 0x02,               // System PLL voltage reference
        SysPLLVoltRefExternal = 0x00,       // 0 = use external voltage reference
        SysPLLVolRefPLLBlock = 0x02,        // 1 = use PLL voltage reference block
        PixPLLVoltRefBlockPower = 0x04,     // Pixel PLL voltage reference block power down.
        PixPLLRefBlockPowerDown = 0x00,     // 0 = Power Down
        PixPLLRefBlockPowerUp = 0x04,       // 1 = Power Up
        PixPLLVoltRef = 0x08,               // Pixel PLL voltage reference
        PixPLLVoltRefExternal = 0x00,       // 0 = use external voltage reference
        PixPLLVoltRefPLLBlock = 0x08,       // 1 = use PLL voltage reference block
        DACVoltRefBlockPower = 0x10,        // DAC voltage reference block power down.
        DACRefBlockPowerDown = 0x00,        // 0 = Power Down
        DACRefBlockPowerUp = 0x10,          // 1 = Power Up
        DACVoltRef = 0x20,                  // DAC voltage reference
        DACVoltRefExternal = 0x00,          // 0 = use external voltage reference
        DACVoltRefBlock = 0x20              // 1 = use DAC voltage reference block
    };

    namespace Shift
    {
        enum
        {
            SysPLLVoltRefBlockPower = 0x00,
            SysPLLVoltRef = 0x01,
            PixPLLVoltRefBlockPower = 0x02,
            PixPLLVoltRef = 0x03,
            DACVoltRefBlockPower = 0x04,
            DACVoltRef = 0x05
        };
    }

}

}