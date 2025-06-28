//Copyright 2025-Present riplin

#include <dpmi.h>
#include <hag/system/pit.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/modeset.h>
#include <hag/drivers/vga/extmsapi.h>

namespace Hag::VGA::Data
{
    extern uint8_t Font8x8[];
    extern uint8_t Font8x14[];
    extern uint8_t Font8x16[];

    extern uint16_t Font8x8Size;
    extern uint16_t Font8x14Size;
    extern uint16_t Font8x16Size;
}

namespace Hag::VGA::ModeSetting
{

__dpmi_meminfo s_Apertures[8] = { 0 };

bool DeclareAperture(uint32_t address, uint32_t size)
{
    for (uint32_t i = 0; i < sizeof(s_Apertures) / sizeof(__dpmi_meminfo); ++i)
    {
        if ((s_Apertures[i].address == 0) && (s_Apertures[i].size == 0))
        {
            s_Apertures[i].address = address;
            s_Apertures[i].size = size;
            return __dpmi_physical_address_mapping(&s_Apertures[i]) == 0;
        }
    }
    return false;
}

static bool s_Initialized = false;
static IAllocator* s_Allocator = nullptr;
static uint16_t s_FontSelector = 0;
FARPointer s_Font8x8;
FARPointer s_Font8x8Graphics;
FARPointer s_Font8x14;
FARPointer s_Font8x16;
FARPointer s_SystemFont;
FARPointer s_SystemFontGraphics;

bool Initialize(IAllocator& allocator)
{
    using namespace Hag::System;

    if (!s_Initialized)
    {
        s_Allocator = &allocator;
        uint16_t fontSegment = 0;
        if (s_Allocator->AllocDosMem(Data::Font8x8Size + Data::Font8x14Size + Data::Font8x16Size, s_FontSelector, fontSegment))
        {
            s_Font8x8.Set(fontSegment, 0x0000);
            s_Font8x8Graphics.Set(fontSegment, Data::Font8x8Size >> 1);
            s_Font8x14.Set(fontSegment, Data::Font8x8Size);
            s_Font8x16.Set(fontSegment, Data::Font8x8Size + Data::Font8x14Size);
            memcpy(s_Font8x8.ToPointer<uint8_t>(Data::Font8x8Size), Data::Font8x8, Data::Font8x8Size);
            memcpy(s_Font8x14.ToPointer<uint8_t>(Data::Font8x14Size), Data::Font8x14, Data::Font8x14Size);
            memcpy(s_Font8x16.ToPointer<uint8_t>(Data::Font8x16Size), Data::Font8x16, Data::Font8x16Size);

            SYS_ClearInterrupts();
            s_SystemFont = InterruptTable::Pointer<InterruptTable::CharacterTable>();
            s_SystemFontGraphics = InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>();
            SYS_RestoreInterrupts();

            s_Initialized = true;

            return External::Initialize(allocator);
        }
    }
    return s_Initialized;
}

void Shutdown()
{
    using namespace Hag::System;

    if (s_Initialized)
    {
        External::Shutdown();

        for (uint32_t i = 0; i < sizeof(s_Apertures) / sizeof(__dpmi_meminfo); ++i)
        {
            if ((s_Apertures[i].address != 0) && (s_Apertures[i].size != 0))
            {
                __dpmi_free_physical_address_mapping(&s_Apertures[i]);
            }        
        }

        SYS_ClearInterrupts();
        InterruptTable::Pointer<InterruptTable::CharacterTable>() = s_SystemFont;
        InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = s_SystemFontGraphics;
        SYS_RestoreInterrupts();

        s_Allocator->FreeDosMem(s_FontSelector);
        s_Initialized = false;
    }
}

void EnumerateVideoModes(const VideoModeCallback_t& callback)
{

    External::IterateModeDescriptors([callback](const ModeDescriptor& descriptor, SetVideoError_t error)
    {
        if (error != SetVideoError::Success)
            return true;

        return callback(descriptor.Width, descriptor.Height, descriptor.Stride, descriptor.Bpp, descriptor.Flags & Flags::PublicFlags, descriptor.Segment);
    });

}

static const ModeDescriptor* GetModeDescriptor(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate, SetVideoError_t& error)
{
    error = SetVideoError::UnknownMode;
    const ModeDescriptor* returnDescriptor = nullptr;

    //Little bit sanity checking.
    if ((refreshRate > RefreshRate::DontCare) &&
        ((refreshRate < 50) || (refreshRate > 100)))
    {
        error = SetVideoError::RefreshRateNotSupported;
        return nullptr;
    }

    External::IterateModeDescriptors([&](const ModeDescriptor& descriptor, SetVideoError_t internalError)
    {
        bool ret = true;
        if (descriptor.Width == width &&
            descriptor.Height == height &&
            descriptor.Bpp == bpp &&
            (descriptor.Flags & Flags::PublicFlags) == flags)
        {
            RefreshRate_t currentRefreshRate = (refreshRate == RefreshRate::DontCare) ? descriptor.RefreshRate : refreshRate;

            if (internalError != SetVideoError::Success)
            {
                error = internalError;//Mode exists, but not usable for some reason. perhaps there is another.
            }
            else 
            {
                internalError = External::SupportsRefreshRate(descriptor, currentRefreshRate);
                if (internalError != SetVideoError::Success)
                {
                    error = internalError;
                }
                else
                {
                    error = SetVideoError::Success;
                    returnDescriptor = &descriptor;
                    ret = false;//= stop iteration
                }
            }
        }
        return ret;
    });

    return returnDescriptor;
}

SetVideoError_t HasVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate)
{
    SetVideoError_t error = SetVideoError::Success;
    GetModeDescriptor(width, height, bpp, flags, refreshRate, error);
    return error;
}

static const ModeDescriptor* ConfigureEGAFeatureBitSwitchesAdapter(const ModeDescriptor* descriptor, Hag::System::BDA::VideoModeOptions_t& videoModeOptions)
{
    using namespace Hag::System;

    struct TestValues
    {
        BDA::VideoDisplayDataArea_t VideoDisplayDataArea;
        Register_t CRTControllerIndexPort;
        BDA::DetectedHardware_t DetectedHardware1;
        BDA::DetectedHardware_t DetectedHardware2;
        BDA::VideoModeOptions_t VideoModeOptions;
    };

    static TestValues Monochrome = 
    {
        0x80,
        Register::CRTControllerIndexB,
        0xFF,
        BDA::DetectedHardware::Monochrome80x25,
        BDA::VideoModeOptions::Monochrome
    };

    static TestValues Color =
    {
        0x01,
        Register::CRTControllerIndexD,
        BDA::DetectedHardware::Monochrome80x25,
        0x00,
        BDA::VideoModeOptions::Color
    };

    enum
    {
        KeepOriginal = 0,
        Select80x25x1bppM = 1,
        Select80x25x4bppC = 2
    };

    uint8_t modeSelect = KeepOriginal;

    if (((BDA::VideoDisplayDataArea::Get() & BDA::VideoDisplayDataArea::VGA) != 0) &&
        (descriptor->LegacyMode != BDA::DisplayMode::Get()))
    {
        TestValues& config = (descriptor->ColorOrMonochromeText == BDA::DetectedHardware::Monochrome80x25) ? Monochrome : Color;

        if (BDA::VideoBaseIOPort::Get() != config.CRTControllerIndexPort)
        {
            modeSelect = Select80x25x1bppM;

            if ((BDA::DetectedHardware::Get() & BDA::DetectedHardware::InitialVideoModeMask) != config.DetectedHardware1)
            {
                modeSelect = Select80x25x4bppC;

                if ((BDA::DetectedHardware::Get() & BDA::DetectedHardware::InitialVideoModeMask) >= config.DetectedHardware2)
                {
                    modeSelect = KeepOriginal;

                    videoModeOptions &= ~BDA::VideoModeOptions::Monochrome;
                    videoModeOptions |= config.VideoModeOptions;

                    BDA::EGAFeatureBitSwitches_t bitSwitches = BDA::EGAFeatureBitSwitches::Get();
                    bitSwitches &= BDA::EGAFeatureBitSwitches::AdapterTypeMask;
                    bitSwitches -= BDA::EGAFeatureBitSwitches::MDAColor40x25_2;

                    if (bitSwitches <= BDA::EGAFeatureBitSwitches::CGAMono80x25)
                    {
                        bitSwitches -= BDA::EGAFeatureBitSwitches::MDAHiResEnhanced;

                        uint8_t mask = ~((bitSwitches >> 7) - 1);
                        BDA::EGAFeatureBitSwitches_t adapter = BDA::EGAFeatureBitSwitches::CGAMono80x25_2;

                        if ((bitSwitches != 0) && ((bitSwitches & 0x80) == 0))
                            adapter = BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2;

                        BDA::VideoDisplayDataArea_t displayDataArea = BDA::VideoDisplayDataArea::Get();
                        uint8_t lineMode200 = displayDataArea;

                        displayDataArea &= ~BDA::VideoDisplayDataArea::LineMode200;
                        lineMode200 &= BDA::VideoDisplayDataArea::LineMode200;

                        lineMode200 >>= 7;
                        lineMode200 &= config.VideoDisplayDataArea;

                        adapter -= lineMode200;
                        displayDataArea |= mask & config.VideoDisplayDataArea;

                        BDA::EGAFeatureBitSwitches::Get() &= BDA::EGAFeatureBitSwitches::FeatureConnectorMask;
                        BDA::EGAFeatureBitSwitches::Get() |= adapter;
                        BDA::VideoDisplayDataArea::Get() = displayDataArea;
                    }
                }
            }
        }
    }
    if (modeSelect == Select80x25x4bppC)
    {
        SetVideoError_t error;
        descriptor = GetModeDescriptor(80, 25, BitsPerPixel::Bpp4, Flags::Text | Flags::Color | Flags::Sequential, RefreshRate::R70Hz, error);
    }
    if (modeSelect == Select80x25x1bppM)
    {
        SetVideoError_t error;
        descriptor = GetModeDescriptor(80, 25, BitsPerPixel::Bpp1, Flags::Text | Flags::Monochrome | Flags::Sequential, RefreshRate::R70Hz, error);
    }
    return descriptor;
}

static bool IsMDAHiResEnhanced()
{
    using namespace Hag::System;

    BDA::EGAFeatureBitSwitches_t adapterType = BDA::EGAFeatureBitSwitches::Get() &
                                                BDA::EGAFeatureBitSwitches::AdapterTypeMask;

    return (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced) ||
        (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2);
}

static Scanlines_t GetNumberOfActiveScanlines(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;

    Scanlines_t scanlines = descriptor.Scanlines;

    if (scanlines == Scanlines::Invalid)
    {
        scanlines = Scanlines::S400;

        if ((BDA::VideoDisplayDataArea::Get() & BDA::VideoDisplayDataArea::LineMode400) == 0)
        {
            scanlines = Scanlines::S350;

            if (!IsMDAHiResEnhanced() &&
                (BDA::DisplayMode::Get() != VideoMode::T80x25x1bppM)) //TODO: I don't like this comparison...
            {
                scanlines = Scanlines::S200;
            }
        }
    }
    return scanlines;
}

static void LoadMCGAPalette(const Data::PaletteData& palette)
{
    using namespace Hag::System::BDA;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t greyscale = 0;

    const uint8_t* colors = palette.Colors;

    if ((VideoDisplayDataArea::Get() & 
        (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0x00)
    {
        uint16_t tripleCount = palette.Count * 3;
        for (uint16_t idx = 0; idx < tripleCount; ++idx)
        {
            RAMDACData::Write(*(colors++));
        }
    }
    else
    {
        for (uint16_t i = 0; i < palette.Count; ++i)
        {
            red = *(colors++);
            green = *(colors++);
            blue = *(colors++);
            greyscale = ((0x4D * uint16_t(red)) +
                         (0x97 * uint16_t(green)) +
                         (0x1c * uint16_t(blue)) + 0x80) >> 8;
            RAMDACData::Write(greyscale);
            RAMDACData::Write(greyscale);
            RAMDACData::Write(greyscale);
        }
    }
}

static void LoadEGAPalette(const Data::PaletteData& palette)
{
    const uint8_t* colors = palette.Colors;

    for (uint16_t i = 0; i < palette.Count; ++i)
    {
        uint8_t packedValue = *(colors++) << 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
        packedValue <<= 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
        packedValue <<= 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
    }
}

static void LoadMonochromePalette(const Data::PaletteData& palette)
{
    const uint8_t* colors = palette.Colors;
    for (uint16_t idx = 0; idx < palette.Count; ++idx)
    {
        uint8_t color = *(colors++);
        RAMDACData::Write(color);
        RAMDACData::Write(color);
        RAMDACData::Write(color);
    }
}

static void InitializeRAMDACPalette(const ModeDescriptor& descriptor)
{
    using namespace Hag::System::BDA;

    DACMask::Write(0xFF);

    if (descriptor.Palettes != nullptr)
    {
        const Data::PalettePair* palettePair = descriptor.Palettes;
        while (palettePair->Primary != nullptr)
        {
            const Data::PaletteData* palette = ((palettePair->Alternate != nullptr) && 
                                    ((VideoDisplayDataArea::Get() & palettePair->Primary->Mask) != 0)) ?
                                    palettePair->Alternate : palettePair->Primary;

            DACWriteIndex::Write(palettePair->StartIndex);

            switch(palette->Type)
            {
            case Data::PaletteType::EGA:
                LoadEGAPalette(*palette);
                break;
            case Data::PaletteType::MCGA:
                LoadMCGAPalette(*palette);
                break;
            default:
                LoadMonochromePalette(*palette);
            }
            ++palettePair;
        }
    }
    else
    {
        External::SetupRAMDAC(descriptor);
    }
}

static void InitializePalettes(const ModeDescriptor& descriptor)
{
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        const VideoParameters& parameters = descriptor.GetParameters();
        AttributeControllerData::Write(AttributeController::Register::Palette0, parameters.AttributePalette, 16);
        AttributeController::BorderColor::Write(parameters.Config.BorderColor);
        InitializeRAMDACPalette(descriptor);
    }
}

static Sequencer::ClockingMode_t ToggleScreenOnOff(Sequencer::ClockingMode_t screenToggle)
{
    Sequencer::ClockingMode_t originalClockingMode = Sequencer::ClockingMode::Read();

    screenToggle &= Sequencer::ClockingMode::ScreenOff;

    Sequencer::ClockingMode_t newClockingMode = (originalClockingMode & ~Sequencer::ClockingMode::ScreenOff) | screenToggle;

    Sequencer::ClockingMode::Write(newClockingMode);

    return originalClockingMode;
}

static Sequencer::ClockingMode_t TurnScreenOn()
{
    return ToggleScreenOnOff(Sequencer::ClockingMode::ScreenOn);
}

static Sequencer::ClockingMode_t TurnScreenOff()
{
    return ToggleScreenOnOff(Sequencer::ClockingMode::ScreenOff);
}

static void ApplyParameters(const ModeDescriptor& descriptor, Register_t baseVideoIOPort, RefreshRate_t refreshRate)
{
    using namespace Hag::System;

    SYS_ClearInterrupts();
    
    const VideoParameters& parameters = descriptor.GetParameters();

    Sequencer::Reset::Write(Sequencer::Reset::AsynchronousReset);
    SequencerData::Write(Sequencer::Register::ClockingMode, parameters.Config.Sequencer, sizeof(parameters.Config.Sequencer));
    MiscellaneousOutput_t miscOutput = parameters.GetMiscellaneousOutput();
    if (refreshRate != descriptor.RefreshRate)//Custom timing
    {
        miscOutput |= MiscellaneousOutput::ClockSelect;
    }
    MiscellaneousOutput::Write(miscOutput);
    
    PIT::MiniSleep();

    Sequencer::Reset::Write(Sequencer::Reset::AsynchronousReset | Sequencer::Reset::SynchronousReset);

    SYS_RestoreInterrupts();

    CRTController::VerticalRetraceEnd::Write(baseVideoIOPort, 0x00);

    CRTController::HorizontalTotal::Write(baseVideoIOPort, parameters.Timings.Horizontal.Total);                           //CR00
    CRTController::HorizontalDisplayEnd::Write(baseVideoIOPort, parameters.Timings.Horizontal.DisplayEnd);                 //CR01
    CRTController::StartHorizontalBlank::Write(baseVideoIOPort, parameters.Timings.Horizontal.StartBlankLow);              //CR02
    CRTController::EndHorizontalBlank::Write(baseVideoIOPort, parameters.Timings.Horizontal.EndBlank);                     //CR03
    CRTController::StartHorizontalSyncPosition::Write(baseVideoIOPort, parameters.Timings.Horizontal.StartSyncPosition);   //CR04
    CRTController::EndHorizontalSyncPosition::Write(baseVideoIOPort, parameters.Timings.Horizontal.EndSyncPosition);       //CR05
    CRTController::VerticalTotal::Write(baseVideoIOPort, parameters.Timings.Vertical.Total);                               //CR06
    CRTController::CRTCOverflow::Write(baseVideoIOPort, parameters.Timings.Vertical.Overflow);                             //CR07
    CRTController::PresetRowScan::Write(baseVideoIOPort, parameters.Config.PresetRowScan);                                 //CR08
    CRTController::MaximumScanLine::Write(baseVideoIOPort, parameters.GetMaximumScanline());                               //CR09
    CRTController::CursorStartScanLine::Write(baseVideoIOPort, parameters.Font.CursorStartScanLine);                       //CR0A
    CRTController::CursorEndScanLine::Write(baseVideoIOPort, parameters.Font.CursorEndScanLine);                           //CR0B
    CRTController::StartAddressHigh::Write(baseVideoIOPort, 0);                                                            //CR0C
    CRTController::StartAddressLow::Write(baseVideoIOPort, 0);                                                             //CR0D
    CRTController::CursorLocationAddressHigh::Write(baseVideoIOPort, 0);                                                   //CR0E
    CRTController::CursorLocationAddressLow::Write(baseVideoIOPort, 0);                                                    //CR0F
    CRTController::VerticalRetraceStart::Write(baseVideoIOPort, parameters.Timings.Vertical.RetraceStart);                 //CR10
    CRTController::VerticalRetraceEnd::Write(baseVideoIOPort, parameters.GetVerticalRetraceEnd());                         //CR11
    CRTController::VerticalDisplayEnd::Write(baseVideoIOPort, parameters.Timings.Vertical.DisplayEnd);                     //CR12

    if (External::IsExtendedMode(descriptor))                                                                              //CR13
        External::SetExtendedOffset(descriptor);
    else
        CRTController::ScreenOffset::Write(baseVideoIOPort, descriptor.CalculateVGAOffset());

    CRTController::UnderlineLocation::Write(baseVideoIOPort, parameters.Config.UnderlineLocation);                         //CR14
    CRTController::StartVerticalBlank::Write(baseVideoIOPort, parameters.Timings.Vertical.StartBlank);                     //CR15
    CRTController::EndVerticalBlank::Write(baseVideoIOPort, parameters.Timings.Vertical.EndBlank);                         //CR16
    CRTController::CRTCModeControl::Write(baseVideoIOPort, parameters.Config.CRTCModeControl);                             //CR17
    CRTController::LineCompare::Write(baseVideoIOPort, parameters.Config.LineCompare);                                     //CR18

    InputStatus1::Read(baseVideoIOPort + 0x06);
    FeatureControl::Write(baseVideoIOPort + 0x06, 0x00);

    AttributeController::AttributeMode::Write(parameters.Config.AttributeMode);                                            //AR10
    AttributeController::ColorPlane::Write(parameters.Config.ColorPlane);                                                  //AR12
    AttributeController::HorizontalPixelPanning::Write(parameters.Config.HorizontalPixelPanning);                          //AR13

    GraphicsControllerData::Write(GraphicsController::Register::SetResetData, parameters.Config.Graphics, 9);
}

static void ClearScreen(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;

    if ((BDA::VideoModeOptions::Get() & BDA::VideoModeOptions::DontClearDisplay) == 0)
    {
        uint16_t value = ((descriptor.Flags & Flags::Mode) == Flags::Text) ? 0x0720 : 0x0000;

        Sequencer::EnableWritePlane_t writePlane = Sequencer::EnableWritePlane::Read();
        if ((descriptor.Flags & Flags::MemoryOrganization) == Flags::Planar)
        {
            Sequencer::EnableWritePlane::Write(
                Sequencer::EnableWritePlane::Plane1 |
                Sequencer::EnableWritePlane::Plane2 |
                Sequencer::EnableWritePlane::Plane3 |
                Sequencer::EnableWritePlane::Plane4);
        }

        uint16_t pages = 1;
        if ((descriptor.Stride * descriptor.Height) > (64*1024))
        {
            pages = External::GetNumberOf64KBPages();
        }

        uint16_t clearCount = 0x8000;
        if ((descriptor.Segment & 0xF000) == 0xB000)
            clearCount = 0x4000;

        for (uint16_t page = 0; page < pages; ++page)
        {
            External::SelectPage(page);

            uint16_t* ptr = FARPointer(descriptor.Segment, 0x0000).ToPointer<uint16_t>(clearCount << 1);
            if (value != 0)
            {
                for (uint16_t i = 0; i < clearCount; ++i)
                    *ptr++ = value;
            }
            else
            {
                memset(ptr, 0, clearCount << 1);
            }
        }
        External::SelectPage(0);

        if ((descriptor.Flags & Flags::MemoryOrganization) == Flags::Planar)
        {
            Sequencer::EnableWritePlane::Write(writePlane);
        }
    }
}

static void ConfigureFontLoadMemoryMapping()
{
    Sequencer::EnableWritePlane::Write(Sequencer::EnableWritePlane::Plane3);
    Sequencer::MemoryModeControl::Write(
        Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        Sequencer::MemoryModeControl::SequentialAddressingMode);
    
    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::Plane3);
    GraphicsController::GraphicsControllerMode::Write(GraphicsController::GraphicsControllerMode::Mode0);
    GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);
}

static void ConfigureTextMemoryMapping()
{
    using namespace Hag::System;

    Sequencer::EnableWritePlane::Write(Sequencer::EnableWritePlane::Plane1 | Sequencer::EnableWritePlane::Plane2);
    Sequencer::MemoryModeControl::Write(Sequencer::MemoryModeControl::ExtendedMemoryAddress);

    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::Plane1);
    GraphicsController::GraphicsControllerMode::Write(GraphicsController::GraphicsControllerMode::OddEvenAddressing);
    if (BDA::VideoBaseIOPort::Get() == Register::CRTControllerIndexD)
        GraphicsController::MemoryMapModeControl::Write(
            GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
            GraphicsController::MemoryMapModeControl::B8000HtoBFFFFH);
    else
        GraphicsController::MemoryMapModeControl::Write(
            GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
            GraphicsController::MemoryMapModeControl::B0000HtoB7FFFH);
}

static void ApplyGraphicsCharacterSetOverride()
{
    using namespace Hag::System;
    if (!BDA::VideoParameterControlBlockPointer::Get().IsNull())
    {
        BDA::VideoParameterControlBlock* videoParameterControlBlock = 
            BDA::VideoParameterControlBlockPointer::Get().ToPointer<BDA::VideoParameterControlBlock>();
        
        if (!videoParameterControlBlock->GraphicsCharacterSetOverride.IsNull())
        {
            BDA::GraphicsCharacterSet* graphicsCharacterSet = 
                videoParameterControlBlock->GraphicsCharacterSetOverride.ToPointer<BDA::GraphicsCharacterSet>(sizeof(BDA::GraphicsCharacterSet) + 20);
            uint8_t* videoModes = graphicsCharacterSet->ApplicableVideoModes;
            while (*videoModes != 0xFF)
            {
                if (*videoModes == BDA::DisplayMode::Get())
                    break;
                ++videoModes;
            }
            if (*videoModes == BDA::DisplayMode::Get())
            {
                BDA::RowsOnScreen::Get() = graphicsCharacterSet->NumberOfCharacterRowsDisplayed - 1;
                BDA::PointHeightOfCharacterMatrix::Get() = graphicsCharacterSet->CharacterLength;
                SYS_ClearInterrupts();
                InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = graphicsCharacterSet->CharacterFontDefinitionTable;
                SYS_RestoreInterrupts();
            }
        }
    }
}

static void SetInterruptTableFontPointer(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;
    if (GetNumberOfActiveScanlines(descriptor) != Scanlines::S200)
    {
        SYS_ClearInterrupts();
        InterruptTable::Pointer<InterruptTable::CharacterTable>() = s_Font8x16;
        SYS_RestoreInterrupts();
    }
    BDA::CursorScanLines::Get().End = 0;
    BDA::CursorScanLines::Get().Start = 0;
    ApplyGraphicsCharacterSetOverride();
}

static void UploadFont(const Data::FontConfiguration& fontConfig)
{
    static uint8_t Bank[] = { 0x00, 0x40, 0x80, 0xC0, 0x20, 0x60, 0xA0, 0xE0 };

    uint16_t offset = 0;
    uint16_t count =fontConfig.CharacterCount + 1;
    const uint8_t* font = fontConfig.Font;
    do
    {
        FARPointer ptr(0xA000, offset + (uint16_t(Bank[fontConfig.BankIndex]) << 8));            
        uint8_t span = fontConfig.CharacterHeight == 0 ? 16 : fontConfig.CharacterHeight;

        do
        {
            if ((fontConfig.CharacterHeight == 0) &&
                ((uint8_t(count) == 0x68) ||
                (uint8_t(count) == 0x87) ||
                (uint8_t(count) == 0x8F) ||
                (uint8_t(count) == 0x90) ||
                (uint8_t(count) == 0x96) ||
                (uint8_t(count) == 0x99)))
            {
                memcpy(ptr.ToPointer<uint8_t>(8), font, 8);
                ptr.Offset += 8;
                font += 9;

                memcpy(ptr.ToPointer<uint8_t>(7), font, 7);
                ptr.Offset += 8;
                font += 7;
            }
            else
            {
                memcpy(ptr.ToPointer<uint8_t>(span), font, span);
                ptr.Offset += span;
                font += span;
            }

            ptr.Offset += 32 - span;
            --count;

        } while (count != 0);

        if (!fontConfig.Patch)
            break;

        offset = uint16_t(*font) << 5;
        ++font;
        ++count;
    } while (offset != 0);
}

static void ApplyMode(const ModeDescriptor& descriptor, Hag::System::BDA::VideoModeOptions_t videoModeOptions, RefreshRate_t refreshRate)
{
    using namespace Hag::System;

    BDA::DisplayMode::Get() = descriptor.LegacyMode;
    BDA::VideoModeOptions::Get() = videoModeOptions;
    BDA::VideoBaseIOPort::Get() = descriptor.CrtController;

    External::DisableExtendedMode();

    BDA::ActiveDisplayNumber::Get() = 0;
    BDA::VideoBufferOffset::Get() = 0;
    BDA::CursorPositions::Clear();

    const VideoParameters& parameters = descriptor.GetParameters();

    BDA::NumberOfScreenColumns::Get() = parameters.Config.NumCharacterColumns;
    BDA::RowsOnScreen::Get() = parameters.Config.NumScreenRowsMinus1;
    BDA::PointHeightOfCharacterMatrix::Get() = parameters.Config.CharacterLength;

    if (!External::IsExtendedMode(descriptor))
        BDA::VideoBufferSize::Get() = parameters.Config.VideoBufferSize;

    BDA::CursorScanLines::Get().Start = parameters.Font.CursorStartScanLine;
    BDA::CursorScanLines::Get().End = parameters.Font.CursorEndScanLine;

    ApplyParameters(descriptor, BDA::VideoBaseIOPort::Get(), refreshRate);

    AttributeController::PixelPadding::Write(0);
    
    if (External::IsExtendedMode(descriptor))
        External::ApplyExtendedModeSettings(descriptor);

    InitializePalettes(descriptor);

    ClearScreen(descriptor);

    if ((descriptor.Flags & Flags::Mode) == Flags::Text)
    {
        if (parameters.Font.Font != nullptr)
        {
            ConfigureFontLoadMemoryMapping();
            UploadFont(parameters.Font);
            ConfigureTextMemoryMapping();
        }
    }
    else
        SetInterruptTableFontPointer(descriptor);

    if (External::IsExtendedMode(descriptor))
        External::SetStartAddress(0);

    InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06);
    AttributeControllerIndex::Write(AttributeControllerIndex::EnableVideoDisplay);
    
    if (descriptor.CRTModeControlRegValue != 0xFF)
        BDA::CRTModeControlRegValue::Get() = descriptor.CRTModeControlRegValue;

    if (descriptor.CGAColorPaletteMaskSetting != 0xFF)
        BDA::CGAColorPaletteMaskSetting::Get() = descriptor.CGAColorPaletteMaskSetting;
}

static const ModeDescriptor* s_CurrentDescriptor = nullptr;

SetVideoError_t SetVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate, bool clearDisplay)
{
    using namespace Hag::System;

    SetVideoError_t error = SetVideoError::Success;
    const ModeDescriptor* descriptor = GetModeDescriptor(width, height, bpp, flags, refreshRate, error);
    if (error != SetVideoError::Success)
        return error;

    if (refreshRate == RefreshRate::DontCare)
        refreshRate = descriptor->RefreshRate;

    TurnScreenOff();
    MiscellaneousOutput::Write(MiscellaneousOutput::Read() & ~MiscellaneousOutput::VideoEnable);
    //Turn monitor off
    External::TurnMonitorOff();
    External::WriteExtensionRegisters(*descriptor);

    External::SetupClock(*descriptor, refreshRate);

    BDA::VideoModeOptions_t videoModeOptions = BDA::VideoModeOptions::Get();
    if (clearDisplay)
        videoModeOptions &= ~BDA::VideoModeOptions::DontClearDisplay;
    else
        videoModeOptions |= BDA::VideoModeOptions::DontClearDisplay;

    BDA::DetectedHardware::Get() = 
        (BDA::DetectedHardware::Get() &
        ~BDA::DetectedHardware::InitialVideoModeMask) |
        descriptor->ColorOrMonochromeText;

    descriptor = ConfigureEGAFeatureBitSwitchesAdapter(descriptor, videoModeOptions);

    s_CurrentDescriptor = descriptor;

    SYS_ClearInterrupts();
    InterruptTable::Pointer<InterruptTable::CharacterTable>() = s_Font8x8;
    InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = s_Font8x8Graphics;
    SYS_RestoreInterrupts();

    videoModeOptions &= ~(BDA::VideoModeOptions::Unknown | BDA::VideoModeOptions::Inactive);

    ApplyMode(*descriptor, videoModeOptions, refreshRate);

    //Turn monitor on
    External::TurnMonitorOn();
    TurnScreenOn();

    return SetVideoError::Success;
}

void* GetLinearFrameBuffer()
{
    if ((s_CurrentDescriptor != nullptr) &&
        ((s_CurrentDescriptor->Flags & Flags::LinearFramebuffer) != 0))
        return External::GetLinearFrameBuffer();
    return nullptr;
}


const VideoParameters& ModeDescriptor::GetParameters() const
{
    return ((Flags & Flags::ParameterCount) == Flags::MultiParameter) ? 
        *Parameters[GetNumberOfActiveScanlines(*this)] :
        *Parameters[0];
}

CRTController::ScreenOffset_t ModeDescriptor::CalculateVGAOffset() const
{
    const VideoParameters& parameters = GetParameters();

    uint8_t everySecond = parameters.Config.Sequencer[0] & VGA::Sequencer::ClockingMode::LoadSerializersEverySecondCharacterClock;
    uint8_t everyFourth = parameters.Config.Sequencer[0] & VGA::Sequencer::ClockingMode::LoadSerializersEveryFourthCaracterClock;
    uint8_t charWidth = (everyFourth != 0) ? 4 : ((everySecond != 0) ? 2 : 1);

    return CRTController::ScreenOffset_t((parameters.Timings.Horizontal.DisplayEnd + 1) / (charWidth * 2));
}

}