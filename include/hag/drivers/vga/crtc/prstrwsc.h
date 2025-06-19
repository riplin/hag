//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag::VGA::CRTController
{

namespace Register
{

enum
{
    PresetRowScan = 0x08,                                   //P_R_SCAN CR8
};

}

typedef uint8_t PresetRowScan_t;

namespace PresetRowScan
{
    enum //Mask
    {
        PresetRowScanCount = 0x1F,      //Value = starting row within a character cell for the first character row displayed after vertical retrace.
                                        //        This allows a partial character row to be displayed at the top of the display and is used for scrolling.
        BytePan = 0x60                  //Value = number of bytes to pan.
                                        //        The number ofpixels to pan is specified in AR13.
    };

    namespace Shift
    {
        enum
        {
            PresetRowScanCount = 0x00,
            BytePan = 0x05
        };
    }

    inline PresetRowScan_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::PresetRowScan);
        return PresetRowScan_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, PresetRowScan_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::PresetRowScan, CRTControllerData_t(value));
    }

}

}
