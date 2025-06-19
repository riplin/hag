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
    CursorEndScanLine = 0x0B,                               //CESL CRB
};

}

typedef uint8_t CursorEndScanLine_t;

namespace CursorEndScanLine
{
    enum //Mask
    {
        CursorScanLineEnd = 0x1F,       //Value = ending scan line number within the character cell for the text cursor.
                                        //        If the value of the cursor start scan line is greater than the value of the cursor end line,
                                        //        then no cursor is generated.
        CursorSkew = 0x60               //CSR-SKW These bits control the delay skew of the cursor signal. Cursor skew delays the text cursor
                                        //        by the selected number of clocks. For example, a skew of 1 moves the cursor right one character position on the screen.
                                        //        00 = Zero character clock skew
                                        //        01 = One character clock skew
                                        //        10 = Two character clock skew
                                        //        11 = Three character clock skew
    };

    namespace Shift
    {
        enum
        {
            CursorScanLineEnd = 0x00,
            CursorSkew = 0x05
        };
    }

    inline CursorEndScanLine_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::CursorEndScanLine);
        return CursorEndScanLine_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, CursorEndScanLine_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::CursorEndScanLine, CRTControllerData_t(value));
    }

}

}
