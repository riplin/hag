//Copyright 2025-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/matrox/shared/data.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>
#include <hag/drivers/matrox/shared/crtcext/hrcntext.h>
#include <hag/drivers/matrox/shared/crtcext/vrcntext.h>

namespace Hag::Matrox::Shared::Function::ModeSetting
{

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

namespace Flags //Internal flags in addition to the public flags.
{
    enum
    {
        PublicFlags = 0x00FF,
        MAFCSelection = 0x4000,
        MAFCVGA = 0x0000,
        MAFCDisable = 0x4000,
        ParameterCount = 0x8000,
        MultiParameter = 0x8000,
        SingleParameter = 0x0000,
    };
}

#pragma pack(push, 1)
struct HorizontalTimings
{
    VGA::CRTController::HorizontalTotal_t Total; //CR00
    VGA::CRTController::HorizontalDisplayEnd_t DisplayEnd; //CR01
    VGA::CRTController::StartHorizontalBlank_t StartBlankLow; //CR02
    VGA::CRTController::EndHorizontalBlank_t EndBlank; //CR03
    VGA::CRTController::StartHorizontalSyncPosition_t StartSyncPosition; //CR04
    VGA::CRTController::EndHorizontalSyncPosition_t EndSyncPosition; //CR05
};

struct VerticalTimings
{
    VGA::CRTController::VerticalTotal_t Total; //CR06
    VGA::CRTController::CRTCOverflow_t Overflow; //CR07
    VGA::CRTController::MaximumScanLine_t VBlank; //CR09 (partial)
    VGA::CRTController::VerticalRetraceStart_t RetraceStart; //CR10
    VGA::CRTController::VerticalRetraceEnd_t RetraceEnd; //CR11
    VGA::CRTController::VerticalDisplayEnd_t DisplayEnd; //CR12
    VGA::CRTController::StartVerticalBlank_t StartBlank; //CR15
    VGA::CRTController::EndVerticalBlank_t EndBlank; //CR16
};

struct ResolutionTimings
{
    uint32_t FrequencyKHz;
    CRTCExtension::HorizontalCounterExtensions_t HorizontalCounterExtensions;
    CRTCExtension::VerticalCounterExtensions_t VerticalCounterExtensions;    
    VGA::MiscellaneousOutput_t Polarity;// (partial, polarity)
    VGA::CRTController::MaximumScanLine_t Convert2t4; //CR09 (partial)
    HorizontalTimings& Horizontal;
    VerticalTimings& Vertical;
};

struct Configuration
{
    uint8_t NumCharacterColumns;
    uint8_t NumScreenRowsMinus1;
    uint8_t CharacterLength;
    uint16_t VideoBufferSize;
    VGA::MiscellaneousOutput_t MiscellaneousOutput;
    VGA::SequencerData_t Sequencer[4]; //SR01 - SR04
    VGA::CRTController::PresetRowScan_t PresetRowScan; //CR08
    VGA::CRTController::MaximumScanLine_t MaximumScanLine; //CR09 The character height comes from font.
    VGA::CRTController::VerticalRetraceEnd_t VerticalRetraceEnd; //CR11 partial
    VGA::CRTController::UnderlineLocation_t UnderlineLocation; //CR14
    VGA::CRTController::CRTCModeControl_t CRTCModeControl; //CR17
    VGA::CRTController::LineCompare_t LineCompare; //CR18
    VGA::GraphicsControllerData_t* Graphics; //GR00 - GR08
    VGA::AttributeController::AttributeMode_t AttributeMode; //AR10
    VGA::AttributeController::BorderColor_t BorderColor; //AR11
    VGA::AttributeController::ColorPlane_t ColorPlane; //AR12
    VGA::AttributeController::HorizontalPixelPanning_t HorizontalPixelPanning; //AR13
    uint8_t CompressedPalette[];
};

struct FontConfiguration
{
    uint8_t* Font;
    uint8_t CharacterCount; //Add 1 (16 bit)
    uint8_t CharacterHeight;
    uint8_t BankIndex;
    bool Patch;
    VGA::CRTController::MaximumScanLine_t RenderHeight; //CR09
    VGA::CRTController::CursorStartScanLine_t CursorStartScanLine; //CR0A
    VGA::CRTController::CursorEndScanLine_t CursorEndScanLine; //CR0B
};

struct VideoParameters
{
    ResolutionTimings& Timings;
    Configuration& Config;
    VGA::AttributeControllerData_t* AttributePalette;
    FontConfiguration& Font;

    inline VGA::MiscellaneousOutput_t GetMiscellaneousOutput()
    {
        return Config.MiscellaneousOutput | Timings.Polarity;
    }

    inline VGA::CRTController::MaximumScanLine_t GetMaximumScanline()
    {
        return Config.MaximumScanLine | Timings.Convert2t4 | Timings.Vertical.VBlank | Font.RenderHeight;
    }

    inline VGA::CRTController::VerticalRetraceEnd_t GetVerticalRetraceEnd()
    {
        return Config.VerticalRetraceEnd | Timings.Vertical.RetraceEnd;
    }

    bool IsExtendedMode();
    bool IsInterlaced();
    bool HorizontalSyncPolarityPositive();
    bool VerticalSyncPolarityPositive();
    uint8_t CharacterClockInPixels();
    uint8_t ScanlineDouble();//Returns 0 if no doubling, 1 if there is.
    float RefreshRateHz();

    uint32_t HorizontalTotalChars();
    uint32_t HorizontalDisplayEnableEndChars();
    uint32_t HorizontalBlankStartChars();
    uint32_t HorizontalBlankEndChars();
    uint32_t HorizontalSyncStartChars();
    uint32_t HorizontalSyncEndChars();

    uint32_t HorizontalTotalPixels();
    uint32_t HorizontalDisplayEnableEndPixels();
    uint32_t HorizontalBlankStartPixels();
    uint32_t HorizontalBlankEndPixels();
    uint32_t HorizontalSyncStartPixels();
    uint32_t HorizontalSyncEndPixels();

    uint32_t VerticalDisplayEnableEndLines();
    uint32_t VerticalTotalLines();
    uint32_t VerticalBlankStartLines();
    uint32_t VerticalBlankEndLines();
    uint32_t VerticalSyncStartLines();
    uint32_t VerticalSyncEndLines();
};

struct ModeDescriptor
{
    uint16_t Width;
    uint16_t Height;
    BitsPerPixel_t Bpp;
    Flags_t Flags;
    RefreshRate_t RefreshRate;
    VGA::VideoMode_t LegacyMode; //Do not use this for anything other than the BDA!
    uint16_t Segment;
    Hag::VGA::Register_t CrtController;
    Hag::System::BDA::DetectedHardware_t ColorOrMonochromeText;
    Scanlines_t Scanlines;
    Hag::System::BDA::CRTModeControlRegValue_t CRTModeControlRegValue; //0xFF = don't set
    Hag::System::BDA::CGAColorPaletteMaskSetting_t CGAColorPaletteMaskSetting; //0xFF = don't set
    Data::PalettePair* Palettes;
    VideoParameters** Parameters; //4 if multi parameters, 1 if not.

    VideoParameters& GetParameters() const;
    uint16_t CalculateOffset() const;
};

#pragma pack(pop)

}

namespace Hag::Matrox::Shared::Data
{

extern Function::ModeSetting::ModeDescriptor s_Descriptors[];
extern uint32_t s_NumDescriptors;

}
