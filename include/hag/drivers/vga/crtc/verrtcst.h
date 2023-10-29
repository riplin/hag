//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t VerticalRetraceStart_t;

namespace VerticalRetraceStart
{
    enum //Mask
    {
        VerticalRetraceStartLow = 0xFF      //11 bit value = number of scan lines at which VSYNC becomes active.
                                            //These are the low-order 8 bits. Bit 8 is bit 2 of CR7, bit 9 is bit 7 of CR7, bit 10 is bit 4 of CR5E.
    };

    namespace Shift
    {
        enum
        {
            VerticalRetraceStartLow = 0x00
        };
    }

    inline VerticalRetraceStart_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::VerticalRetraceStart);
        return VerticalRetraceStart_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, VerticalRetraceStart_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::VerticalRetraceStart, CRTControllerData_t(value));
    }

    inline uint8_t GetVerticalRetraceStartLow(VerticalRetraceStart_t value)
    {
        return (value & VerticalRetraceStartLow) >> Shift::VerticalRetraceStartLow;
    }
    
    inline VerticalRetraceStart_t SetVerticalRetraceStartLow(uint8_t value)
    {
        return (value << Shift::VerticalRetraceStartLow) & VerticalRetraceStartLow;
    }

}

}}}
