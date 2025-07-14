//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <has/types.h>
#include <has/ialloc.h>
#include <hag/system/bda.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/modeset.h>

//See bottom of file for exension points.

namespace Hag::VGA::Data
{

#pragma pack(push, 1)

typedef uint8_t PaletteType_t;
namespace PaletteType
{
    enum
    {
        EGA = 0x00,
        MCGA = 0x01,
        Monochrome = 0x02,
    };
}

struct PaletteData
{
    System::BDA::VideoDisplayDataArea_t Mask;

    PaletteType_t Type;
    uint16_t Count;
    uint8_t Colors[];
};

struct PalettePair
{
    const PaletteData* Primary;
    const PaletteData* Alternate;
    uint8_t StartIndex;
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

    extern PalettePair EGAPair[];
    extern PalettePair CGAPair[];
    extern PalettePair HerculesPair[];
    extern PalettePair MCGAPairs[];
    extern uint8_t Font8x8[];
    extern uint8_t Font8x14[];
    extern uint8_t Font8x16[];

    extern FontConfiguration FontConfig00;// = { Font8x8, 0xFF, 0x08, 0x00, false, 0x07, 0x06, 0x07 };
    extern FontConfiguration FontConfig01;// = { Font8x16, 0xFF, 0x00, 0x00, false, 0x0D, 0x0B, 0x0C };
    extern FontConfiguration FontConfig02;// = { Font8x16, 0xFF, 0x10, 0x00, true, 0x0F, 0x0D, 0x0E };
    extern FontConfiguration FontConfig03;// = { Font8x16, 0xFF, 0x10, 0x00, true, 0x07, 0x06, 0x07 };
    extern FontConfiguration FontConfig04;// = { Font8x16, 0xFF, 0x00, 0x00, true, 0x0D, 0x0B, 0x0C };
    extern FontConfiguration FontConfig04b;// = { Font8x14, 0xFF, 0x0E, 0x00, true, 0x0D, 0x0B, 0x0C };
    extern FontConfiguration FontConfig05;// = { Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x00, 0x00 };
    extern FontConfiguration FontConfig06;// = { Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x00, 0x00 };
    extern FontConfiguration FontConfig07;// = { Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x20, 0x00 };
    extern FontConfiguration FontConfig08;// = { Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x20, 0x00 };
    extern FontConfiguration FontConfig09;// = { Font8x8, 0xFF, 0x08, 0x00, true, 0x01, 0x00, 0x00 };
    extern FontConfiguration FontConfig10;// = { Font8x8, 0xFF, 0x08, 0x00, false, 0x00, 0x00, 0x00 };
    extern FontConfiguration FontConfig11;// = { Font8x14, 0xFF, 0x0E, 0x00, true, 0x00, 0x00, 0x00 };
    extern FontConfiguration FontConfig12;// = { Font8x16, 0xFF, 0x10, 0x00, true, 0x00, 0x00, 0x00 };
    extern FontConfiguration FontConfig13;// = { Font8x14, 0xFF, 0x09, 0x00, true, 0x08, 0x07, 0x08 };
}

namespace Hag::VGA::ModeSetting
{

extern bool DeclareAperture(uint32_t address, uint32_t size);

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
        PublicFlags =           0x0000FFFF,
        ParameterCount =        0x80000000,
        SingleParameter =       0x00000000,
        MultiParameter =        0x80000000,

        //These flags can be used by drivers.
        DriverFlag0 =           0x00080000,
        DriverFlag1 =           0x00040000,
        DriverFlag2 =           0x00020000,
        DriverFlag3 =           0x00010000,
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

struct VideoParameters
{
    const ResolutionTimings& Timings;
    const Configuration& Config;
    const VGA::AttributeControllerData_t* AttributePalette;
    const Data::FontConfiguration& Font;

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
    uint16_t Stride;
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

#pragma pack(pop)

// External refrerences to internal mode setting state.

extern FARPointer s_Font8x8;
extern FARPointer s_Font8x8Graphics;
extern FARPointer s_Font8x16;
extern FARPointer s_SystemFont;
extern FARPointer s_SystemFontGraphics;
extern const ModeDescriptor* s_CurrentDescriptor;

// These functions need to be implemented by any driver wishing to provide video mode setting.
// Once those functions are implemented, simply linking in the modeset.cpp file will provide proper
// mode setting.

namespace External
{
    extern bool Initialize(Has::IAllocator& allocator);
    extern void Shutdown();

    extern SetVideoError_t SupportsRefreshRate(const ModeDescriptor&descriptor, RefreshRate_t refreshRate);

    extern bool IsExtendedMode(const ModeDescriptor& descriptor);

    typedef std::function<bool(const ModeDescriptor& descriptor, SetVideoError_t error)> DescriptorCallback_t;

    extern void IterateModeDescriptors(const DescriptorCallback_t& callback);
    extern void SetupRAMDAC(const ModeDescriptor& descriptor);
    extern void SetExtendedOffset(const ModeDescriptor& descriptor);

    extern uint16_t GetNumberOf64KBPages();
    extern void SelectPage(uint16_t page);

    extern void DisableExtendedMode();
    extern void ApplyExtendedModeSettings(const ModeDescriptor& descriptor);
    extern void SetStartAddress(uint32_t startAddress);

    extern void TurnMonitorOff();
    extern void TurnMonitorOn();

    extern void WriteExtensionRegisters(const ModeDescriptor& descriptor);

    extern void SetupClock(const ModeDescriptor& descriptor, RefreshRate_t refreshRate);

    extern void* GetLinearFrameBuffer();
    extern SetVideoError_t SetupBuffers(Buffers_t buffers);
    extern void SwapScreen2D(bool waitForVSync);
}

}

namespace Hag::VGA::Data
{

    //Iterates the standard VGA modes + extra VGA modes.
    extern bool IterateModeDescriptors(const ModeSetting::External::DescriptorCallback_t& callback);

}