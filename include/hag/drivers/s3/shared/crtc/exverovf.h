//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    ExtendedVerticalOverflow = 0x5E,                        //EXT_V_OVF CR5E
};

}

typedef uint8_t ExtendedVerticalOverflow_t;

namespace ExtendedVerticalOverflow
{

    enum //Mask
    {
        VerticalTotalHigh = 0x01,           //Bit 10 of VerticalTotal (C6)
        VerticalDisplayEndHigh = 0x02,      //Bit 10 of VerticalDisplayEnd (CR12)
        StartVerticalBlankHigh = 0x04,      //Bit 10 of StartVerticalBlank (CR15)
        VerticalRetraceStartHigh = 0x10,    //Bit 10 of VerticalRetraceStart (CR10)
        LineComparePositionHigh = 0x40      //Bit 10 of LineComparePosition (CR18)
    };

    namespace Shift
    {
        enum
        {
            VerticalTotalHigh = 0x00,
            VerticalDisplayEndHigh = 0x01,
            StartVerticalBlankHigh = 0x02,
            VerticalRetraceStartHigh = 0x04,
            LineComparePositionHigh = 0x06
        };
    }

    inline ExtendedVerticalOverflow_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedVerticalOverflow);
        return ExtendedVerticalOverflow_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedVerticalOverflow_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedVerticalOverflow, VGA::CRTControllerData_t(value));
    }

}

}}}}
