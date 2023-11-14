//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t CRTCOverflow_t;

namespace CRTCOverflow
{
    enum //Mask
    {
        VerticalTotalHigh1 = 0x01,          //Bit 8 of the VerticalTotal register (CR6)
        VerticalDisplayEndHigh1 = 0x02,     //Bit 8 of the VerticalDisplayEnd register (CR12)
        VerticalRetraceStartHigh1 = 0x04,   //Bit 8 of the VerticalRetraceStart register (CR10)
        StartVerticalBlankHigh = 0x08,      //Bit 8 of the StartVerticalBlank register (CR15)
        LineCompareHigh = 0x10,             //Bit 8 of the LineCompare register (CR18)
        VerticalTotalHigh2 = 0x20,          //Bit 9 of the VerticalTotal register (CR6)
        VerticalDisplayEndHigh2 = 0x40,     //Bit 9 of the VerticalDisplayEnd register (CR12)
        VerticalRetraceStartHigh2 = 0x80,   //Bit 9 of the VerticalRetraceStart register (CR10)
    };

    namespace Shift
    {
        enum
        {
            VerticalTotalHigh1 = 0x00,
            VerticalDisplayEndHigh1 = 0x01,
            VerticalRetraceStartHigh1 = 0x02,
            StartVerticalBlankHigh = 0x03,
            LineCompareHigh = 0x04,
            VerticalTotalHigh2 = 0x05,
            VerticalDisplayEndHigh2 = 0x06,
            VerticalRetraceStartHigh2 = 0x07,
        };
    }

    inline CRTCOverflow_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::CRTCOverflow);
        return CRTCOverflow_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, CRTCOverflow_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::CRTCOverflow, CRTControllerData_t(value));
    }

}

}}}
