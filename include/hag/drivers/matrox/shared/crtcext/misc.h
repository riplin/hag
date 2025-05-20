//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/crtcext/data.h>
#include <hag/drivers/matrox/shared/crtc/regtype.h>

namespace Hag { namespace Matrox { namespace Shared { namespace CRTCExtension
{

namespace Register
{

enum
{
    Miscellaneous = 0x03,                         //CER3
};

}

typedef Shared::CRTCExtensionData_t Miscellaneous_t;

namespace Miscellaneous
{
    
    enum //Mask
    {
        ClockScalingFactor = 0x07,      //Video clock scaling factor. Specifies the video clock division factor in MGA mode
        ScaleDiv1 = 0x00,               // 000 = /1
        ScaleDiv2 = 0x01,               // 001 = /2
        ScaleDiv3 = 0x02,               // 010 = /3
        ScaleDiv4 = 0x03,               // 011 = /4
                                        // 100 = Reserved
        ScaleDiv6 = 0x05,               // 101 = /6
                                        // 110 = Reserved
        ScaleDiv8 = 0x07,               // 111 = /8
        Slow256 = 0x02,                 // 0 = Direct frame buffer accesses in mode 13h are accelerated.
                                        // 1 = Mode 13h frame buffer accesses are not acclerated.
                                        //     Unless otherwise specified, this bit should always be 0.
        CompositeSyncEnable = 0x40,     // 0 = Horizontal Sync
                                        // 1 = Composite Sync (Block sync)
        MGAModeEnable = 0x80            // 0 = VGA Compatibility mode. In this mode, VGA data is sent to the DAC via
                                        //     the VGA attribute controller. The memory address counter clock will be selected
                                        //     by the count2 (CRTC17<3> and count4 (CRTC14<5>) bits. This mode
                                        //     should be used for all VGA modes up to mode 13, and for all Super VGA alpha
                                        //     modes. When mgamode = ‘0’, the full frame buffer aperture mapped to
                                        //     MGABASE2 is unusable.
                                        // 1 = MGA Mode. In this mode, the graphics engine data is sent directly to the
                                        //     DAC. The memory address counter is clocked, depending on the state of the
                                        //     hzoom field of the XZOOMCTRL register. This mode should be used for all
                                        //     Super VGA graphics modes and all accelerated graphics modes.
    };

    namespace Shift
    {
        enum
        {
            ClockScalingFactor = 0x00,
            Slow256 = 0x05,
            CompositeSyncEnable = 0x06,
            MGAModeEnable = 0x07
        };
    }

    inline Miscellaneous_t Read()
    {
        Shared::CRTCExtensionIndex::Write(Register::Miscellaneous);
        return Miscellaneous_t(Shared::CRTCExtensionData::Read());
    }

    inline void Write(Miscellaneous_t value)
    {
        Shared::CRTCExtensionData::Write(Register::Miscellaneous, Shared::CRTCExtensionData_t(value));
    }

}

}}}}