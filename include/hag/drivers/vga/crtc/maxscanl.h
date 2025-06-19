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
    MaximumScanLine = 0x09,                                 //MAX_S_LN CR9
};

}

typedef uint8_t MaximumScanLine_t;

namespace MaximumScanLine
{
    enum //Mask
    {
        MaximumScanLineCount = 0x1F,        //Value = (number of scan lines per character row) - 1
        StartVerticalBlankHigh = 0x20,      //SVB 9 Bit 9 of the StartVerticalBlank register (CR15)
        LineCompareHigh = 0x40,             //LCM 9 Bit 9 of the LineCompare register (CR18)
        DoubleScan = 0x80                   //DBL SCN 0 = Normal operation, 1 = Enables double scanning operation.
                                            //            Each line is displayed twice by repeating the row scan counter and video memory address.
                                            //            Vertical parameters in the CRT controller are not affected.
    };

    namespace Shift
    {
        enum
        {
            MaximumScanLineCount = 0x00,
            StartVerticalBlankHigh = 0x05,
            LineCompareHigh = 0x06,
            DoubleScan = 0x07
        };
    }

    inline MaximumScanLine_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::MaximumScanLine);
        return MaximumScanLine_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, MaximumScanLine_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::MaximumScanLine, CRTControllerData_t(value));
    }

}

}
