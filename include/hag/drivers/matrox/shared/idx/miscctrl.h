//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    MiscellaneousControl = 0x1E
};

}

typedef uint8_t MiscellaneousControl_t;

namespace MiscellaneousControl
{
    enum //Mask
    {
        DACPower = 0x01,            // DAC power down. This field is used to remove power from the DACs, to conserve power.
        DACPowerDown = 0x00,        // 0 = DAC disabled (default)
        DACPowerUp = 0x01,          // 1 = DAC enabled
        MAFCFuncSelect = 0x06,      // Matrox Advanced Feature Connector (MAFC) function select. Selects the mode in
                                    // which the MAFC will operate.
        StandardVGA = 0x00,         // 00 = standard VGA connector (output only)
                                    //      In standard VGA mode (this is the reset mode), the VD<15:8> outputs
                                    //      are tri-stated, and straps on these pins can be input to the chip.
        MAFC = 0x02,                // 01 = Matrox Advanced Feature Connector
                                    //      In MAFC mode, the output just before the DAC, plus the 8-bit alpha
                                    //      channel are output on the 16-bit feature connector using both edges of
                                    //      the clock. This effectively transfers one 32-bit pixel RGBA/8888 per
                                    //      clock. The alpha portion is only valid in 32 bit/pixel display modes
                                    //      (ColorDepth = ‘100’ / Bits32O8p). (See the ColorDepth field in MultiplexControl.)
                                    //      Note: The alpha portion does not pass through the LUT.
        MAFCDisable = 0x06,         // 11 = disable feature connector
                                    //      When the feature connector is disabled, the VD<15:8> outputs are tri-
                                    //      stated, and the VD<7:0>, VBLANK/, and VDCLK pins are driven low,
                                    //      regardless of the state of the VEVIDEO, VESYNC, and VEDCLK
                                    //      pins (that is, it is not possible to tri-state VD<7:0>, VBLANK/, and
                                    //      VDCLK when MAFCFuncSelect = ‘11’ / MAFCDisable).
        VGADACBitDepth = 0x08,      // VGA 8-bit DAC. This field is used for compatibility with standard VGA, which uses a
                                    // 6-bit DAC.
        VGADAC6Bit = 0x00,          // 0 = 6 bit palette (default)
        VGADAC8Bit = 0x08,          // 1 = 8 bit palette
        LUTPower = 0x10,            // LUT RAM chip select. Used to power up the LUT.
        LUTPowerDown = 0x00,        // 0 =  LUT disabled
        LUTPowerUp = 0x10           // 1 = LUT enabled
    };

    namespace Shift
    {
        enum
        {
            DACPower = 0x00,
            MAFCFuncSelect = 0x01,
            VGADACBitDepth = 0x03,
            LUTPower = 0x04,
        };
    }
}

}