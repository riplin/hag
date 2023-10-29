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

    inline uint1_t GetVerticalTotalHigh1(CRTCOverflow_t value)
    {
        return (value & VerticalTotalHigh1) >> Shift::VerticalTotalHigh1;
    }

    inline uint1_t GetVerticalDisplayEndHigh1(CRTCOverflow_t value)
    {
        return (value & VerticalDisplayEndHigh1) >> Shift::VerticalDisplayEndHigh1;
    }

    inline uint1_t GetVerticalRetraceStartHigh1(CRTCOverflow_t value)
    {
        return (value & VerticalRetraceStartHigh1) >> Shift::VerticalRetraceStartHigh1;
    }

    inline uint1_t GetStartVerticalBlankHigh(CRTCOverflow_t value)
    {
        return (value & StartVerticalBlankHigh) >> Shift::StartVerticalBlankHigh;
    }

    inline uint1_t GetLineCompareHigh(CRTCOverflow_t value)
    {
        return (value & LineCompareHigh) >> Shift::LineCompareHigh;
    }

    inline uint1_t GetVerticalTotalHigh2(CRTCOverflow_t value)
    {
        return (value & VerticalTotalHigh2) >> Shift::VerticalTotalHigh2;
    }

    inline uint1_t GetVerticalDisplayEndHigh2(CRTCOverflow_t value)
    {
        return (value & VerticalDisplayEndHigh2) >> Shift::VerticalDisplayEndHigh2;
    }

    inline uint1_t GetVerticalRetraceStartHigh2(CRTCOverflow_t value)
    {
        return (value & VerticalRetraceStartHigh2) >> Shift::VerticalRetraceStartHigh2;
    }

    inline CRTCOverflow_t SetVerticalTotalHigh1(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::VerticalTotalHigh1) & VerticalTotalHigh1);
    }
    
    inline CRTCOverflow_t SetVerticalDisplayEndHigh1(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::VerticalDisplayEndHigh1) & VerticalDisplayEndHigh1);
    }
    
    inline CRTCOverflow_t SetVerticalRetraceStartHigh1(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::VerticalRetraceStartHigh1) & VerticalRetraceStartHigh1);
    }
    
    inline CRTCOverflow_t SetStartVerticalBlankHigh(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::StartVerticalBlankHigh) & StartVerticalBlankHigh);
    }

    inline CRTCOverflow_t SetLineCompareHigh(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::LineCompareHigh) & LineCompareHigh);
    }

    inline CRTCOverflow_t SetVerticalTotalHigh2(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::VerticalTotalHigh2) & VerticalTotalHigh2);
    }

    inline CRTCOverflow_t SetVerticalDisplayEndHigh2(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::VerticalDisplayEndHigh2) & VerticalDisplayEndHigh2);
    }

    inline CRTCOverflow_t SetVerticalRetraceStartHigh2(uint1_t value)
    {
        return CRTCOverflow_t((value << Shift::VerticalRetraceStartHigh2) & VerticalRetraceStartHigh2);
    }

}

}}}
