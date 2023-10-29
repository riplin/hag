//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/data.h>

namespace Hag { namespace VGA { namespace GraphicsController
{

typedef uint8_t ColorDontCare_t;

//This register is effective in read mode 1, and controls whether the corresponding bit of the
//Color Compare Register is to be ignored or used for color comparison.

namespace ColorDontCare
{
    enum //Mask
    {
        ComparePlaneSelect = 0x0F   //0 = The corresponding color plane becomes a don't care plane
                                    //    when the CPU read from the video memory is performed in
                                    //    read mode 1.
                                    //1 = The corresponding color plane is used for color comparison
                                    //    with the data in the Color Compare register.
    };

    namespace Shift
    {
        enum
        {
            ComparePlaneSelect = 0x00
        };
    }

    inline ColorDontCare_t Read()
    {
        GraphicsControllerIndex::Write(GraphicsControllerRegister::ColorDontCare);
        return ColorDontCare_t(GraphicsControllerData::Read());
    }
    
    inline void Write(ColorDontCare_t value)
    {
        GraphicsControllerData::Write(GraphicsControllerRegister::ColorDontCare, GraphicsControllerData_t(value));
    }

    inline uint4_t GetComparePlaneSelect(ColorDontCare_t value)
    {
        return (value & ComparePlaneSelect) >> Shift::ComparePlaneSelect;
    }

    inline ColorDontCare_t SetComparePlaneSelect(uint4_t value)
    {
        return ColorDontCare_t(value << Shift::ComparePlaneSelect) & ComparePlaneSelect;
    }
}

}}}
