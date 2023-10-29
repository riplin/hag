//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace VGA { namespace AttributeControllerRegister
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
    AttributeMode = 0x10,                   //ATR_MODE AR10
    BorderColor = 0x11,                     //BDR_CLR AR11
    ColorPlane = 0x12,                      //DISP_PLN AR12
    HorizontalPixelPanning = 0x13,          //H_PX_PAN AR13
    PixelPadding = 0x14,                    //PX_PADD AR14
};

}}}
