//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline CursorEndScanLine_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::CursorEndScanLine);
        return CursorEndScanLine_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, CursorEndScanLine_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::CursorEndScanLine, CRTControllerData_t(value));
    }

    inline uint5_t GetCursorScanLineEnd(CursorEndScanLine_t value)
    {
        return (value & CursorScanLineEnd) >> Shift::CursorScanLineEnd;
    }
    
    inline uint2_t GetCursorSkew(CursorEndScanLine_t value)
    {
        return (value & CursorSkew) >> Shift::CursorSkew;
    }

    inline CursorEndScanLine_t SetCursorScanLineEnd(uint5_t value)
    {
        return CursorEndScanLine_t((value << Shift::CursorScanLineEnd) & CursorScanLineEnd);
    }

    inline CursorEndScanLine_t SetCursorSkew(uint2_t value)
    {
        return CursorEndScanLine_t((value << Shift::CursorSkew) & CursorSkew);
    }

}

}}}