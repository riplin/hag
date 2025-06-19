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
    VerticalRetraceStart = 0x10,                            //VRS CR10
};

}

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

    inline VerticalRetraceStart_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::VerticalRetraceStart);
        return VerticalRetraceStart_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, VerticalRetraceStart_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::VerticalRetraceStart, CRTControllerData_t(value));
    }

}

}
