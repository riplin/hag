//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/modeset.h>
#include <hag/system/bda.h>
#include <functional>

//See bottom of file for exension points.

namespace Hag::VGA::Data
{

#pragma pack(push, 1)

struct PaletteData
{
    System::BDA::VideoDisplayDataArea_t Mask;
    uint8_t Flags;
    uint16_t Count;
    uint8_t Colors[];
};

struct PalettePair
{
    const PaletteData* Primary;
    const PaletteData* Alternate;
    uint8_t StartIndex;
};

#pragma pack(pop)

}

namespace Hag::VGA::ModeSetting
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
        ParameterCount = 0x8000,
        MultiParameter = 0x8000,
        SingleParameter = 0x0000,

        //These flags can be used by drivers.
        DriverFlag0 = 0x0800,
        DriverFlag1 = 0x0400,
        DriverFlag2 = 0x0200,
        DriverFlag3 = 0x0100,
    };
}

#pragma pack(push, 1)

struct HorizontalTimings
{
    CRTController::HorizontalTotal_t Total; //CR00
    CRTController::HorizontalDisplayEnd_t DisplayEnd; //CR01
    CRTController::StartHorizontalBlank_t StartBlankLow; //CR02
    CRTController::EndHorizontalBlank_t EndBlank; //CR03
    CRTController::StartHorizontalSyncPosition_t StartSyncPosition; //CR04
    CRTController::EndHorizontalSyncPosition_t EndSyncPosition; //CR05
};

struct VerticalTimings
{
    CRTController::VerticalTotal_t Total; //CR06
    CRTController::CRTCOverflow_t Overflow; //CR07
    CRTController::MaximumScanLine_t VBlank; //CR09 (partial)
    CRTController::VerticalRetraceStart_t RetraceStart; //CR10
    CRTController::VerticalRetraceEnd_t RetraceEnd; //CR11
    CRTController::VerticalDisplayEnd_t DisplayEnd; //CR12
    CRTController::StartVerticalBlank_t StartBlank; //CR15
    CRTController::EndVerticalBlank_t EndBlank; //CR16
};

struct ResolutionTimings
{
    const HorizontalTimings& Horizontal;
    VerticalTimings& Vertical;
    MiscellaneousOutput_t Polarity;// (partial, polarity)
    CRTController::MaximumScanLine_t Convert2t4; //CR09 (partial)
};

struct Configuration
{
    uint8_t NumCharacterColumns;
    uint8_t NumScreenRowsMinus1;
    uint8_t CharacterLength;
    uint16_t VideoBufferSize;
    MiscellaneousOutput_t MiscellaneousOutput;
    SequencerData_t Sequencer[4]; //SR01 - SR04
    CRTController::PresetRowScan_t PresetRowScan; //CR08
    CRTController::MaximumScanLine_t MaximumScanLine; //CR09 The character height comes from font.
    CRTController::VerticalRetraceEnd_t VerticalRetraceEnd; //CR11 partial
    CRTController::UnderlineLocation_t UnderlineLocation; //CR14
    CRTController::CRTCModeControl_t CRTCModeControl; //CR17
    CRTController::LineCompare_t LineCompare; //CR18
    const GraphicsControllerData_t* Graphics; //GR00 - GR08
    AttributeController::AttributeMode_t AttributeMode; //AR10
    AttributeController::BorderColor_t BorderColor; //AR11
    AttributeController::ColorPlane_t ColorPlane; //AR12
    AttributeController::HorizontalPixelPanning_t HorizontalPixelPanning; //AR13
    uint8_t CompressedPalette[];
};

struct FontConfiguration
{
    const uint8_t* Font;
    uint8_t CharacterCount; //Add 1 (16 bit)
    uint8_t CharacterHeight;
    uint8_t BankIndex;
    bool Patch;
    VGA::CRTController::MaximumScanLine_t RenderHeight; //CR09
    VGA::CRTController::CursorStartScanLine_t CursorStartScanLine; //CR0A
    VGA::CRTController::CursorEndScanLine_t CursorEndScanLine; //CR0B
};

#pragma pack(pop)

struct VideoParameters
{
    const ResolutionTimings& Timings;
    const Configuration& Config;
    const VGA::AttributeControllerData_t* AttributePalette;
    const FontConfiguration& Font;

    inline VGA::MiscellaneousOutput_t GetMiscellaneousOutput() const
    {
        return Config.MiscellaneousOutput | Timings.Polarity;
    }

    inline VGA::CRTController::MaximumScanLine_t GetMaximumScanline() const
    {
        return Config.MaximumScanLine | Timings.Convert2t4 | Timings.Vertical.VBlank | Font.RenderHeight;
    }

    inline VGA::CRTController::VerticalRetraceEnd_t GetVerticalRetraceEnd() const
    {
        return Config.VerticalRetraceEnd | Timings.Vertical.RetraceEnd;
    }
};

struct ModeDescriptor
{
    uint16_t Width;
    uint16_t Height;
    BitsPerPixel_t Bpp;
    Flags_t Flags;
    RefreshRate_t RefreshRate;
    VGA::VideoMode_t LegacyMode; //This value is set in the BDA.
    uint16_t Segment;
    Hag::VGA::Register_t CrtController;
    Hag::System::BDA::DetectedHardware_t ColorOrMonochromeText;
    Scanlines_t Scanlines;
    Hag::System::BDA::CRTModeControlRegValue_t CRTModeControlRegValue; //0xFF = don't set
    Hag::System::BDA::CGAColorPaletteMaskSetting_t CGAColorPaletteMaskSetting; //0xFF = don't set
    const Data::PalettePair* Palettes;
    const VideoParameters** Parameters; //4 if multi parameters, 1 if not.

    const VideoParameters& GetParameters() const;
    CRTController::ScreenOffset_t CalculateVGAOffset() const; //Provides default offset calculation for standard VGA modes.
};


// These functions need to be implemented by any driver wishing to provide video mode setting.
// Once those functions are implemented, simply linking in the modeset.cpp file will provide proper
// mode setting.

namespace External
{
    extern bool Initialize();
    extern void Shutdown();
    extern bool IsExtendedMode(const ModeDescriptor& modeDescriptor);

    typedef std::function<bool(const ModeDescriptor& descriptor, SetVideoError_t error)> DescriptorCallback_t;

    extern void IterateModeDescriptors(const DescriptorCallback_t& callback);
    extern void SetupRAMDAC(const ModeDescriptor& descriptor);
    extern void SetExtendedOffset(const ModeDescriptor& descriptor);

    extern uint16_t GetNumberOf64KBPages();
    extern void SelectPage(uint16_t page);

    extern void UploadFont(const FontConfiguration& fontConfig);//Memory config is set up. Only upload.
    extern const FARPointer& Get8x8Font();
    extern const FARPointer& Get8x8GraphicsFont();
    extern const FARPointer& Get8x16Font();

    extern void DisableExtendedMode();
    extern void ApplyExtendedModeSettings(const ModeDescriptor& descriptor);
    extern void SetStartAddress(uint32_t startAddress);

    extern void TurnMonitorOff();
    extern void TurnMonitorOn();

    extern void WriteExtensionRegisters(const VideoParameters& parameters);

    extern void SetupClock(const VideoParameters& parameters);
}

}