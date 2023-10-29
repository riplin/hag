//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/data.h>

namespace Hag { namespace VGA { namespace AttributeController
{

typedef uint8_t PixelPadding_t;

//This register specifies the high-order bits of video output when pixel padding is enabled and disabled in the 256 color mode.

namespace PixelPadding
{
    enum //Mask
    {
        PaddingV5V4 = 0x03, //These bits are enabled with a logical 1 of bit 7 of AR10,
                            //and can be used in place of the V5 and V4 bits from the
                            //Palette registers to form the 8-bit digital color value
                            //output.
        PaddingV7V6 = 0x0C  //In all modes except 256 color mode, these bits are the
                            //two high-order bits of the 8-bit digital color value
                            //output.
    };

    namespace Shift
    {
        enum
        {
            PaddingV5V4 = 0x00,
            PaddingV7V6 = 0x02
        };
    }

    inline PixelPadding_t Read()
    {
        AttributeControllerIndex::Write(AttributeControllerRegister::PixelPadding);
        return PixelPadding_t(AttributeControllerData::Read());
    }
    
    inline void Write(PixelPadding_t value)
    {
        AttributeControllerData::Write(AttributeControllerRegister::PixelPadding, AttributeControllerData_t(value));
    }

    inline uint2_t GetPaddingV5V4(PixelPadding_t value)
    {
        return (value & PaddingV5V4) >> Shift::PaddingV5V4;
    }

    inline uint2_t GetPaddingV7V6(PixelPadding_t value)
    {
        return (value & PaddingV7V6) >> Shift::PaddingV7V6;
    }

    inline PixelPadding_t SetPaddingV5V4(uint2_t value)
    {
        return PixelPadding_t(value << Shift::PaddingV5V4) & PaddingV5V4;
    }

    inline PixelPadding_t SetPaddingV7V6(uint2_t value)
    {
        return PixelPadding_t(value << Shift::PaddingV7V6) & PaddingV7V6;
    }
}

}}}
