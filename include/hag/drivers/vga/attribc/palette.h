//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/regtype.h>
#include <hag/drivers/vga/attribc/data.h>

namespace Hag::VGA::AttributeController
{

namespace Register
{

enum
{
    Palette0 = 0x00,                        //PLT_REG AR00
    Palette1 = 0x01,                        //PLT_REG AR01
    Palette2 = 0x02,                        //PLT_REG AR02
    Palette3 = 0x03,                        //PLT_REG AR03
    Palette4 = 0x04,                        //PLT_REG AR04
    Palette5 = 0x05,                        //PLT_REG AR05
    Palette6 = 0x06,                        //PLT_REG AR06
    Palette7 = 0x07,                        //PLT_REG AR07
    Palette8 = 0x08,                        //PLT_REG AR08
    Palette9 = 0x09,                        //PLT_REG AR09
    PaletteA = 0x0A,                        //PLT_REG AR0A
    PaletteB = 0x0B,                        //PLT_REG AR0B
    PaletteC = 0x0C,                        //PLT_REG AR0C
    PaletteD = 0x0D,                        //PLT_REG AR0D
    PaletteE = 0x0E,                        //PLT_REG AR0E
    PaletteF = 0x0F,                        //PLT_REG AR0F
};

}

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
        AttributeControllerIndex::Write(Register::Palette0 + index);
        return Palette_t(AttributeControllerData::Read());
    }
    
    inline void Write(uint5_t index, Palette_t value)
    {
        AttributeControllerData::Write(Register::Palette0 + index, AttributeControllerData_t(value));
    }

}

}
