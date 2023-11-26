//Copyright 2023-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/funcs.h>
#include <hag/drivers/vga/dacdata.h>
#include <hag/drivers/vga/dacmask.h>
#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/attribc/index.h>
#include <hag/drivers/vga/crtc/index.h>


namespace Hag { namespace VGA
{

void EnableVideoDisplay()
{
    using namespace Hag::System::BDA;

    Register_t inputStatus = VideoBaseIOPort::Get() + (VGA::Register::InputStatus1D - VGA::Register::CRTControllerIndexD);
    AttributeControllerIndex::ResetIndex(inputStatus);
    AttributeControllerIndex::Write(AttributeControllerIndex::EnableVideoDisplay);
}

uint8_t* DecompressPaletteColor(uint8_t* paletteData, uint8_t& red, uint8_t& green, uint8_t& blue)
{
    uint8_t redSettings = *paletteData & 0xC0;//Top 2 bits hold compression information
    red = *paletteData & 0x3F;//6 bit color data
    ++paletteData;

    if (redSettings == 0)
    {
        uint8_t greenSettings = *paletteData & 0x40;
        green = *paletteData & 0x3F;//6 bit color data
        ++paletteData;
        if (greenSettings == 0)
        {
            blue = *paletteData;
            ++paletteData;
        }
        else
        {
            blue = green;
        }
    }
    else if (redSettings == 0x40)
    {
        green = red;
        blue = *paletteData;
        ++paletteData;
    }
    else if (redSettings == 0x80)
    {
        green = blue = red;
    }
    else
    {
        blue = red;
        green = *paletteData;
        ++paletteData;
    }
    return paletteData;
}

uint16_t SetPaletteColor(uint16_t colorIndex, uint8_t red, uint8_t green, uint8_t blue)
{
    SYS_ClearInterrupts();
    DACWriteIndex::Write(colorIndex);
    RAMDACData::Write(red);
    RAMDACData::Write(green);
    RAMDACData::Write(blue);
    SYS_RestoreInterrupts();

    return colorIndex + 1;
}

uint16_t ApplyPalette(uint16_t colorIndex, uint16_t count, uint8_t* paletteData)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    for (uint16_t i = 0; i < count; ++i)
    {
        paletteData = DecompressPaletteColor(paletteData, red, green, blue);
        colorIndex = SetPaletteColor(colorIndex, red, green, blue);
    }
    return colorIndex;
}

void Set248ColorPalette()
{
    using namespace Hag::System::BDA;

    uint16_t colorIndex = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
 
    colorIndex = ApplyPalette(colorIndex, 16, ((VideoDisplayDataArea::Get() &
                                               (VideoDisplayDataArea::GrayScale |
                                                VideoDisplayDataArea::MonochromeMonitor)) == 0) ?
                                                Data::ColorPalette : Data::MonochromePalette);

    colorIndex = ApplyPalette(colorIndex, 16, Data::SecondPalette);

    if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
    {
        ApplyPalette(colorIndex, 216, Data::Color216Palette);
    }
    else
    {
        uint8_t* palettePtr = Data::Color216Palette;
        for (int i = 0; i < 216; ++i)
        {
            palettePtr = DecompressPaletteColor(palettePtr, red, green, blue);
            red = green = blue = ((blue * 36043 + green * 19333 + red * 9830 + 16384) >> 15) & 0xFF;
            colorIndex = SetPaletteColor(colorIndex, red, green, blue);
        }
    }
}

void SetPalette(VideoMode_t mode, bool ifVesaIsColor, bool ifVesaIs256Color)
{
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        if (DACMask::Read() != 0xFF)
            DACMask::Write(0xFF);

        bool paletteNotSet = false;
        if ((mode == VideoMode::T80x25x2M) ||
            (mode == VideoMode::G640x350x2M))
        {
            ApplyPalette(0, 64, Data::Monochrome64Palette);
        }
        else if (mode == VideoMode::G320x200x256C)
        {
            Set248ColorPalette();
        }
        else if (mode > VideoMode::MaxValid)
        {
            if (!ifVesaIsColor)
            {
                ApplyPalette(0, 64, Data::Monochrome64Palette);
            }
            else if (ifVesaIs256Color)
            {
                Set248ColorPalette();
            }
            else paletteNotSet = true;
        }
        else paletteNotSet = true;

        if (paletteNotSet)
        {
            bool paletteSet3 = false;
            if (mode < VideoMode::G320x200x4C)
            {
                EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

                paletteSet3 = (((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2));
            }
            else
            {
                paletteSet3 = ((mode != VideoMode::Unknown1) && (mode <= VideoMode::G640x200x16C));
            }

            if (paletteSet3)
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, Data::ColorPalette3);
                }
                else
                {
                    ApplyPalette(0, 64, Data::MonochromePalette3);
                }
            }
            else
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, Data::ColorPalette2);
                }
                else
                {
                    ApplyPalette(0, 64, Data::MonochromePalette2);
                }
            }
        }
    }
}

}}
