//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/bda.h>
#include <hag/drivers/matrox/shared/crtcext/hrcntext.h>
#include <hag/drivers/matrox/shared/crtcext/vrcntext.h>

namespace Hag::Matrox::Shared::Function
{

namespace System
{
    bool Initialize();
    void Shutdown();
}

namespace Mode
{
    typedef uint8_t BitsPerPixel_t;
    namespace BitsPerPixel
    {
        enum
        {
            Bpp1 = 0x01,
            Bpp2 = 0x02,
            Bpp4 = 0x04,
            Bpp8 = 0x08,
            Bpp15 = 0x0F,
            Bpp16 = 0x10,
            Bpp24 = 0x18,
            Bpp32 = 0x20
        };
    }

    typedef uint16_t Flags_t;
    namespace Flags
    {
        enum
        {
            Mode = 0x01,                //Mode type is text or graphics
            Text = 0x01,
            Graphics = 0x00,

            Chromacity = 0x02,          //Monochrome or color
            Monochrome = 0x02,
            Color = 0x00,

            MemoryOrganization = 0x04,  //Sequential or planar memory organization, only applies to graphical modes
            Sequential = 0x00,
            Planar = 0x04,

            LinearFramebuffer = 0x08,   //Linear framebuffer access
        };
    }

    namespace Flags //Internal flags in addition to the public flags.
    {
        enum
        {
            MAFCSelection = 0x4000,
            MAFCVGA = 0x0000,
            MAFCDisable = 0x4000,
            ParameterCount = 0x8000,
            MultiParameter = 0x8000,
            SingleParameter = 0x0000,
        };
    }


    typedef uint8_t RefreshRate_t;
    namespace RefreshRate
    {
        enum
        {
            DontCare = 0,
            R60Hz = 60,
            R70Hz = 70,
            R72Hz = 72
        };
    }

    bool Has(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare);
    bool Set(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare, bool clearDisplay = true);

    uint8_t GetMemoryIn64KBlocks();
    bool IsExtensionReg7Writeable();

    void PrintNewModeSettings();

    typedef uint8_t Scanlines_t;
    namespace Scanlines
    {
        enum
        {
            Invalid = 0xFF,
            S200 = 0,
            S350 = 1,
            S400 = 2,
            S480 = 3
        };
    }
    
    #pragma pack(push, 1)
    struct PaletteData
    {
        Hag::System::BDA::VideoDisplayDataArea_t Mask;
        uint8_t Flags;
        uint16_t Count;
        uint8_t Colors[];
    };

    struct PalettePair
    {
        PaletteData* Primary;
        PaletteData* Alternate;
        uint8_t StartIndex;
    };

    struct ParameterFontPair
    {
        Hag::System::BDA::VideoParameterTable* VideoParameters;
        uint8_t* Font;
        uint16_t CharacterCount;
        uint8_t CharacterHeight;
        uint8_t BankIndex;
        bool Patch;
    };

    struct VideoMode
    {
        uint16_t Width;
        uint16_t Height;
        BitsPerPixel_t Bpp;
        Flags_t Flags;
        RefreshRate_t RefreshRate;
        VGA::VideoMode_t LegacyMode; //Do not use this for anything other than the BDA!
        uint16_t Segment;
        uint16_t ClearCount;
        Hag::VGA::Register_t CrtController;
        Hag::System::BDA::DetectedHardware_t ColorOrMonochromeText;
        ParameterFontPair* ParametersAndFonts; //4 if multi parameters, 1 if not.
        Scanlines_t Scanlines;
        Hag::System::BDA::CRTModeControlRegValue_t CRTModeControlRegValue; //0xFF = don't set
        Hag::System::BDA::CGAColorPaletteMaskSetting_t CGAColorPaletteMaskSetting; //0xFF = don't set
        PalettePair* Palettes;
        uint32_t MemSizeKB;
        uint32_t FrequencyKHz;
        Hag::Matrox::Shared::CRTCExtension::HorizontalCounterExtensions_t HorizontalCounterExtensions;
        Hag::Matrox::Shared::CRTCExtension::VerticalCounterExtensions_t VerticalCounterExtensions;
    };
    #pragma pack(pop)

    extern VideoMode s_VideoModes[];
    extern uint32_t s_VideoModeCount;
}

}