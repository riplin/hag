//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

//This register is used to implement a split screen function. 
//When the scan line counter value is equal to the content of this register,
//the memory address counter is cleared to O. The linear address counter then 
//sequentially addresses the display buffer starting at address O.
//Each subsequent row address is determined by the addition of the Offset (CR13) register content.
//Bit 8 is bit 4 of CR7. Bit 9 is bit 6 of CR9. Bit 10 is bit 6 of CR5E.


typedef uint8_t LineCompare_t;

namespace LineCompare
{
    enum //Mask
    {
        LineComparePosition = 0xFF  //11-bit Value = number of scan lines at which the screen is
                                    //split into screen A and screen B. This register contains
                                    //the least significant 8 bits of this value.

    };

    namespace Shift
    {
        enum
        {
            LineComparePosition = 0x00,
        };
    }

    inline LineCompare_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::LineCompare);
        return LineCompare_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, LineCompare_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::LineCompare, CRTControllerData_t(value));
    }

}

}}}
