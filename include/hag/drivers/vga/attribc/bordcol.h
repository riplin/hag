//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/regtype.h>
#include <hag/drivers/vga/attribc/data.h>

namespace Hag { namespace VGA { namespace AttributeController
{

namespace Register
{

enum
{
    BorderColor = 0x11,                     //BDR_CLR AR11
};

}

typedef uint8_t BorderColor_t;

//

namespace BorderColor
{
    enum //Mask
    {
        Color = 0xFF    //Bits 7-0 Border Color. This 8-bit register determines the border color displayed
                        //on the CRT screen. The border is an area around the screen display area.
                        //This register is only effective in 8-bit PA modes (CR67_4= 0). See also CR33_5.
    };

    namespace Shift
    {
        enum
        {
            Color = 0x00
        };
    }

    inline BorderColor_t Read()
    {
        AttributeControllerIndex::Write(Register::BorderColor);
        return BorderColor_t(AttributeControllerData::Read());
    }
    
    inline void Write(BorderColor_t value)
    {
        AttributeControllerData::Write(Register::BorderColor, AttributeControllerData_t(value));
    }

}

}}}
