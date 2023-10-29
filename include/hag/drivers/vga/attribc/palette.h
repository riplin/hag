//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/data.h>

namespace Hag { namespace VGA { namespace AttributeController
{

//These are 16, 6-bit registers pointed to by the index and color code.
//They allow a dynamic mapping between the text attribute or graphics color input
//and the display color on the CRT screen.

typedef uint8_t Palette_t;

namespace Palette
{
    enum //Mask
    {
        PrimaryRGB = 0x07,  //The six bit display color, bits 5-0 are output as SR, SG/I, SB/V, R, G and B, respectively.
        SecondaryRGB = 0x38
    };

    namespace Shift
    {
        enum
        {
            PrimaryRGB = 0x00,
            SecondaryRGB = 0x03
        };
    }

    inline Palette_t Read(uint5_t index)
    {
        AttributeControllerIndex::Write(AttributeControllerRegister::Palette0 + index);
        return Palette_t(AttributeControllerData::Read());
    }
    
    inline void Write(uint5_t index, Palette_t value)
    {
        AttributeControllerData::Write(AttributeControllerRegister::Palette0 + index, AttributeControllerData_t(value));
    }

    inline uint3_t GetPrimaryRGB(Palette_t value)
    {
        return (value & PrimaryRGB) >> Shift::PrimaryRGB;
    }

    inline uint3_t GetSecondaryRGB(Palette_t value)
    {
        return (value & SecondaryRGB) >> Shift::SecondaryRGB;
    }
    
    inline Palette_t SetPrimaryRGB(uint3_t value)
    {
        return Palette_t((value << Shift::PrimaryRGB) & PrimaryRGB);
    }

    inline Palette_t SetSecondaryRGB(uint3_t value)
    {
        return Palette_t((value << Shift::SecondaryRGB) & SecondaryRGB);
    }
    
}

}}}
