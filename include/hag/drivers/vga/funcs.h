//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/vidmodes.h>

namespace Hag { namespace VGA
{
    void EnableVideoDisplay();
    uint8_t* DecompressPaletteColor(uint8_t* paletteData, uint8_t& red, uint8_t& green, uint8_t& blue);
    uint16_t SetPaletteColor(uint16_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
    uint16_t ApplyPalette(uint16_t colorIndex, uint16_t count, uint8_t* paletteData);
    void Set248ColorPalette();
    void SetPalette(VideoMode_t mode, bool ifVesaIsColor, bool ifVesaIs256Color);
}}
