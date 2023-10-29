//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline MaximumScanLine_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::MaximumScanLine);
        return MaximumScanLine_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, MaximumScanLine_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::MaximumScanLine, CRTControllerData_t(value));
    }

    inline uint5_t GetMaximumScanLineCount(MaximumScanLine_t value)
    {
        return (value & MaximumScanLineCount) >> Shift::MaximumScanLineCount;
    }

    inline uint1_t GetStartVerticalBlankHigh(MaximumScanLine_t value)
    {
        return (value & StartVerticalBlankHigh) >> Shift::StartVerticalBlankHigh;
    }

    inline uint1_t GetLineCompareHigh(MaximumScanLine_t value)
    {
        return (value & LineCompareHigh) >> Shift::LineCompareHigh;
    }

    inline uint1_t GetDoubleScan(MaximumScanLine_t value)
    {
        return (value & DoubleScan) >> Shift::DoubleScan;
    }

    inline MaximumScanLine_t SetMaximumScanLineCount(uint5_t value)
    {
        return MaximumScanLine_t((value << Shift::MaximumScanLineCount) & MaximumScanLineCount);
    }

    inline MaximumScanLine_t SetStartVerticalBlankHigh(uint1_t value)
    {
        return MaximumScanLine_t((value << Shift::StartVerticalBlankHigh) & StartVerticalBlankHigh);
    }

    inline MaximumScanLine_t SetLineCompareHigh(uint1_t value)
    {
        return MaximumScanLine_t((value << Shift::LineCompareHigh) & LineCompareHigh);
    }

    inline MaximumScanLine_t SetDoubleScan(uint1_t value)
    {
        return MaximumScanLine_t((value << Shift::DoubleScan) & DoubleScan);
    }

}

}}}
