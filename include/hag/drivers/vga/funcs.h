//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/system/bda.h>

namespace Hag { namespace VGA
{

    Register_t GetCRTControllerIndexRegister();

    void EnableVideoDisplay();
    void PrepareAttributeController();

    void ApplyVideoParameters(VideoParameters* parameters);
    void SaveDynamicParameterData(VideoParameters* parameters);
    void SetTextModeBiosData(VideoMode_t mode, VideoParameters* parameters);

    void ClearScreen(VideoMode_t mode, bool ifVesaIsColor);

    void UploadFont(uint8_t* src, uint8_t* dst, uint16_t characters, uint8_t bytesPerCharacter, uint16_t destWidth);
    void PatchFont(uint8_t flags);
    void SetTextFontAndAddressing(uint8_t* font, uint16_t startCharacter, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank);
    void ConfigureCursorPropertiesAndVerticalDisplayEnd(VideoMode_t mode, uint8_t characterPointHeight);
    void SetFont();
    void ConfigureFontAndCursor(VideoMode_t mode, System::BDA::AlphanumericCharSet* fontDefinition);
    void ConfigureFontRamBank(System::BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable* fontDefinition);

    uint8_t* DecompressPaletteColor(uint8_t* paletteData, uint8_t& red, uint8_t& green, uint8_t& blue);
    uint16_t SetPaletteColor(uint16_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
    uint16_t ApplyPalette(uint16_t colorIndex, uint16_t count, uint8_t* paletteData);
    void Set248ColorPalette();
    void SetPalette(VideoMode_t mode, bool ifVesaIsColor, bool ifVesaIs256Color);
    void SetPaletteProfile(Register_t crtcPort);
}}
