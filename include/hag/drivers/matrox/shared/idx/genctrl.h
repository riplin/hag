//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    GeneralControl = 0x1D
};

}

typedef uint8_t GeneralControl_t;

namespace GeneralControl
{
    enum //Mask
    {
        VideoSelect = 0x01,             // Video select. This bit is used by the keying function to select the polarity of the alpha
                                        // comparison in the keying equation (see the ColorKeyHigh register).
                                        // This field must be set to 0 when not in split frame buffer mode. (that is, not in mode
                                        // G16V16 or 2G8V16).
        GfxIfAlpha0OrMasked = 0x00,     // 0 = show graphics stream if alpha is 0 or masked.
        GfxIfAlpha1OrNotMasked = 0x01,  // 1 = show graphics stream if alpha is 1 and not masked.
        VideoAlpha = 0x02,              // Video alpha bit enable. This bit is used by the keying function to enable or disable the
                                        // alpha bits in the equation for split frame buffer modes (mode G16V16 or 2G8V16). It
                                        // is also used in 15-bit single frame buffer mode to enable or disable the 1-bit overlay.
        AlphaDisable = 0x00,            // 0 = disabled (forces the effective value of all alpha bits to 0b) or overlay disable
        AlphaEnable = 0x02,             // 1 = enabled (alpha bits are used for color keying) or overlay enable
        PedestalControl = 0x10,         // Pedestal control. This field specifies whether a 0 or 7.5 IRE blanking pedestal is to be
                                        // generated on the video outputs.
        Pedestal0IRE = 0x00,            // 0 = 0 IRE (default)
        Pedestal7p5IRE = 0x10,          // 1 = 7.5 IRE
        GreenChannelSync = 0x20,        // Green channel sync disable. This field specifies if sync (from the internal signal
                                        // HSYNC) information is to be sent to the output of the green DAC.
                                        // Note: The HSYNC can be programmed to be either horizontal sync only, or composite
                                        // (block) sync. See the csyncen bit of the CRTCEXT3 VGA register.
        GreenChannelSyncEnable = 0x00,  // 0 = enable (default)
        GreenChannelSyncDisable = 0x20  // 1 = disable
    };

    namespace Shift
    {
        enum
        {
            VideoSelect = 0x01,
            VideoAlpha = 0x02,
            PedestalControl = 0x10,
            GreenChannelSync = 0x20,
        };
    }
}

}