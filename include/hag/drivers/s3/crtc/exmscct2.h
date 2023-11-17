//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedMiscellaneousControl2_t;

namespace ExtendedMiscellaneousControl2
{

    enum //Mask
    {
        VClockPhase = 0x01, //0 = VCLK is 180 degrees out of phase with DCLK (inverted)
                            //1 = VCLK is in phase with DCLK
        ColorMode = 0xF0,   //RAMDAC Color Mode
        ColorMode0 = 0x00,  //0000 = Mode 0: 8-bit color, 1 pixel/VCLK
        ColorMode8 = 0x10,  //0001 = Mode 8: 8-bit color, 2 pixels/VCLK
        ColorMode9 = 0x30,  //0011 = Mode 9: 15-bit color, 1 pixel/VCLK
        ColorMode10 = 0x50, //0101 = Mode 10: 16-bit color, 1 pixel/VCLK
        ColorMode12 = 0x70, //0111 = Mode 12: 640x480x24-bit color (packed), 1 pixel/3 DCLKs (Tri032 only)
        ColorMode13 = 0xD0, //1101 = Mode 13: 24-bit color, 1 pixel/VCLK
                            //All other mode values are reserved.
    };

    namespace Shift
    {
        enum
        {
            VClockPhase = 0x00,
            ColorMode = 0x04
        };
    }

    inline ExtendedMiscellaneousControl2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedMiscellaneousControl2);
        return ExtendedMiscellaneousControl2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMiscellaneousControl2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedMiscellaneousControl2, VGA::CRTControllerData_t(value));
    }

}

}}}
