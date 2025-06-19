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
    CursorStartScanLine = 0x0A,                             //CSSL CRA
};

}

typedef uint8_t CursorStartScanLine_t;

namespace CursorStartScanLine
{
    enum //Mask
    {
        CursorScanLineStart = 0x1F,     //Value = (starting cursor row withing the character cell) - 1. When the cursor start register
                                        //        is programmed with a value greater than the cursor end register, no cursor is generated.
        CursorOff = 0x20                //CSR OFF 0 = Turns on the text cursor, 1 = Turns off the text cursor
    };

    namespace Shift
    {
        enum
        {
            CursorScanLineStart = 0x00,
            CursorOff = 0x05
        };
    }

    inline CursorStartScanLine_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::CursorStartScanLine);
        return CursorStartScanLine_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, CursorStartScanLine_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::CursorStartScanLine, CRTControllerData_t(value));
    }

}

}
