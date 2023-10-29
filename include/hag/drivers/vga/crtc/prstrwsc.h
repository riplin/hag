//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline PresetRowScan_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::PresetRowScan);
        return PresetRowScan_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, PresetRowScan_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::PresetRowScan, CRTControllerData_t(value));
    }

    inline uint5_t GetPresetRowScanCount(PresetRowScan_t value)
    {
        return (value & PresetRowScanCount) >> Shift::PresetRowScanCount;
    }
    
    inline uint2_t GetBytePan(PresetRowScan_t value)
    {
        return (value & BytePan) >> Shift::BytePan;
    }

    inline PresetRowScan_t SetPresetRowScanCount(uint5_t value)
    {
        return PresetRowScan_t((value << Shift::PresetRowScanCount) & PresetRowScanCount);
    }
    
    inline PresetRowScan_t SetBytePan(uint2_t value)
    {
        return PresetRowScan_t((value << Shift::BytePan) & BytePan);
    }

}

}}}
