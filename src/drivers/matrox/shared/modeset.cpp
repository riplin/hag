//Copyright 2025-Present riplin

#include <stdio.h>
#include <string.h>
#include "sysintl.h"
#include "modintl.h"

#include <hag/system/bda.h>
#include <hag/system/pit.h>
#include <hag/system/machid.h>
#include <hag/system/keyboard.h>
#include <hag/system/interrup.h>
#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>

#include <hag/drivers/matrox/shared/data.h>
#include <hag/drivers/matrox/shared/miscout.h>              //0x3C2, 0x3CC
#include <hag/drivers/matrox/shared/sqrc/memodctl.h>        //SR4
#include <hag/drivers/matrox/shared/crtcext/adgenext.h>     //CER0
#include <hag/drivers/matrox/shared/crtcext/misc.h>         //CER3
#include <hag/drivers/matrox/shared/crtcext/mempg.h>        //CER4
#include <hag/drivers/matrox/shared/crtcext/hrhlfcnt.h>     //CER5
#include <hag/drivers/matrox/shared/pci/opt.h>              //PCI40
#include <hag/drivers/matrox/shared/pci/idx/curctrl.h>      //IDX06
#include <hag/drivers/matrox/shared/pci/idx/muxctrl.h>      //IDX19
#include <hag/drivers/matrox/shared/pci/idx/pclkctrl.h>     //IDX1A
#include <hag/drivers/matrox/shared/pci/idx/genctrl.h>      //IDX1D
#include <hag/drivers/matrox/shared/pci/idx/miscctrl.h>     //IDX1E
#include <hag/drivers/matrox/shared/pci/idx/pixpllm.h>      //IDX44, IDX48, IDX4C
#include <hag/drivers/matrox/shared/pci/idx/pixplln.h>      //IDX45, IDX49, IDX4D
#include <hag/drivers/matrox/shared/pci/idx/pixpllp.h>      //IDX46, IDX4A, IDX4E
#include <hag/drivers/matrox/shared/pci/idx/pixpllst.h>     //IDX4F

namespace Hag { namespace Matrox { namespace Shared { namespace Function { namespace ModeSetting
{

typedef bool (*DescriptorCallback_t)(ModeDescriptor* descriptor, void* context, SetVideoError_t error);

void IterateModeDescriptors(DescriptorCallback_t callback, void* context)
{
    SetVideoError_t error = SetVideoError::Success;
    for (uint32_t i = 0; i < Data::s_NumDescriptors; ++i)
    {
        error = SetVideoError::Success;

        ModeDescriptor& mode = Data::s_Descriptors[i];

        uint32_t requiredMemory = (mode.Width * mode.Height * mode.Bpp) >> 3;
        if (requiredMemory > (System::s_MemorySize << 10))
        {
            error = SetVideoError::InsufficientVideoMemory;
        }
        else if (((mode.Flags & Flags::ParameterCount) == Flags::SingleParameter) &&//Only legacy modes are multi-parameter.
            (mode.Parameters[0]->Timings.FrequencyKHz >= 220000))//TODO: we should have this number live somewhere.
        {
            error = SetVideoError::NotSupportedByRamdac;
        }
        //TODO monitor filter.

        if (!callback(&Data::s_Descriptors[i], context, error))
            break;
    }
}

typedef bool (*VideoModeCallback_t)(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate, void* context);//Return true to continue receiving modes.

struct ModeEnumeratorContext
{
    VideoModeCallback_t userCallback;
    void* userContext;
};

bool ModeEnumerator(ModeDescriptor* descriptor, void* context, SetVideoError_t error)
{
    ModeEnumeratorContext& ctx = *((ModeEnumeratorContext*)context);
    if (error != SetVideoError::Success)
        return true;

    return ctx.userCallback(descriptor->Width, descriptor->Height, descriptor->Bpp, descriptor->Flags & Flags::PublicFlags, descriptor->RefreshRate, ctx.userContext);
}

void EnumerateVideoModes(VideoModeCallback_t callback, void* context)
{
    ModeEnumeratorContext ctx =
    {
        callback,
        context
    };
    IterateModeDescriptors(ModeEnumerator, &ctx);
}

struct GetModeDescriptorCallbackContext
{
    uint16_t width;
    uint16_t height;
    BitsPerPixel_t bpp;
    Flags_t flags;
    RefreshRate_t refreshRate;
    SetVideoError_t& error;
    ModeDescriptor* result;
};

bool GetModeDescriptorCallback(ModeDescriptor* descriptor, void* context, SetVideoError_t error)
{
    GetModeDescriptorCallbackContext& ctx = *((GetModeDescriptorCallbackContext*)context);
    
    if (descriptor->Width == ctx.width &&
        descriptor->Height == ctx.height &&
        descriptor->Bpp == ctx.bpp &&
        (descriptor->Flags & Flags::PublicFlags) == ctx.flags &&
        ((descriptor->RefreshRate == ctx.refreshRate) || ctx.refreshRate == RefreshRate::DontCare))
    {
        if (error != SetVideoError::Success)
        {
            ctx.error = error;//Mode exists, but not usable for some reason. perhaps there is another.
        }
        else
        {
            ctx.error = SetVideoError::Success;
            ctx.result = descriptor;
            return false;//= stop iteration
        }
    }
    return true;
}

ModeDescriptor* GetModeDescriptor(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate, SetVideoError_t& error)
{
    if (!System::s_Initialized)
    {
        error = SetVideoError::SystemNotInitialized;
        return nullptr;
    }
    
    error = SetVideoError::UnknownMode;

    GetModeDescriptorCallbackContext context =
    {
        width,
        height,
        bpp,
        flags,
        refreshRate,
        error,
        nullptr
    };
    IterateModeDescriptors(GetModeDescriptorCallback, &context);

    return context.result;
}

SetVideoError_t HasVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate)
{
    SetVideoError_t error = SetVideoError::Success;
    GetModeDescriptor(width, height, bpp, flags, refreshRate, error);
    return error;
}

ModeDescriptor* ConfigureEGAFeatureBitSwitchesAdapter(ModeDescriptor* descriptor, Hag::System::BDA::VideoModeOptions_t& videoModeOptions)
{
    using namespace Hag::System;

    struct TestValues
    {
        BDA::VideoDisplayDataArea_t VideoDisplayDataArea;
        VGA::Register_t CRTControllerIndexPort;
        BDA::DetectedHardware_t DetectedHardware1;
        BDA::DetectedHardware_t DetectedHardware2;
        BDA::VideoModeOptions_t VideoModeOptions;
    };

    static TestValues Monochrome = 
    {
        0x80,
        VGA::Register::CRTControllerIndexB,
        0xFF,
        BDA::DetectedHardware::Monochrome80x25,
        BDA::VideoModeOptions::Monochrome
    };

    static TestValues Color =
    {
        0x01,
        VGA::Register::CRTControllerIndexD,
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

bool IsMDAHiResEnhanced()
{
    using namespace Hag::System;

    BDA::EGAFeatureBitSwitches_t adapterType = BDA::EGAFeatureBitSwitches::Get() &
                                                BDA::EGAFeatureBitSwitches::AdapterTypeMask;

    return (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced) ||
        (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2);
}

Scanlines_t GetNumberOfActiveScanlines(const ModeDescriptor& descriptor)
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
                (BDA::DisplayMode::Get() != VGA::VideoMode::T80x25x1bppM)) //TODO: I don't like this comparison...
            {
                scanlines = Scanlines::S200;
            }
        }
    }
    return scanlines;
}

void LoadColorPalette(Data::PaletteData* palette)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t greyscale = 0;

    uint8_t* colors = palette->Colors;

    if ((VideoDisplayDataArea::Get() & 
        (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0x00)
    {
        uint16_t tripleCount = palette->Count * 3;
        for (uint16_t idx = 0; idx < tripleCount; ++idx)
        {
            RAMDACData::Write(*(colors++));
        }
    }
    else
    {
        for (uint16_t i = 0; i < palette->Count; ++i)
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

void LoadCompressedPalette(Data::PaletteData* palette)
{
    using namespace Hag::VGA;

    uint8_t* colors = palette->Colors;

    for (uint16_t i = 0; i < palette->Count; ++i)
    {
        uint8_t packedValue = *(colors++) << 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
        packedValue <<= 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
        packedValue <<= 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
    }
}

void LoadMonochromePalette(Data::PaletteData* palette)
{
    using namespace Hag::VGA;

    uint8_t* colors = palette->Colors;
    for (uint16_t idx = 0; idx < palette->Count; ++idx)
    {
        uint8_t color = *(colors++);
        RAMDACData::Write(color);
        RAMDACData::Write(color);
        RAMDACData::Write(color);
    }
}

void InitializeRAMDACPalette(ModeDescriptor& descriptor)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    DACMask::Write(0xFF);

    if (descriptor.Palettes != nullptr)
    {
        Data::PalettePair* palettePair = descriptor.Palettes;
        while (palettePair->Primary != nullptr)
        {
            Data::PaletteData* palette = ((palettePair->Alternate != nullptr) && 
                                    ((VideoDisplayDataArea::Get() & palettePair->Primary->Mask) != 0)) ?
                                    palettePair->Alternate : palettePair->Primary;

            DACWriteIndex::Write(palettePair->StartIndex);

            if (palette->Flags == 0x00)
            {
                LoadCompressedPalette(palette);
            }
            else if ((palette->Flags & 0x80) == 0x00)
            {
                LoadColorPalette(palette);
            }
            else
            {
                LoadMonochromePalette(palette);
            }
            ++palettePair;
        }
    }
    else
    {
        VGA::DACWriteIndex::Write(0);
        switch(descriptor.Bpp)
        {
        case BitsPerPixel::Bpp32:
            for (int idxcol = 0; idxcol < 256; ++idxcol)
            {
                VGA::RAMDACData::Write(idxcol);
                VGA::RAMDACData::Write(idxcol);
                VGA::RAMDACData::Write(idxcol);
            }
            break;
        case BitsPerPixel::Bpp16:
            for (int idxcol = 0; idxcol < 256; ++idxcol)
            {
                VGA::RAMDACData::Write(idxcol << 3);
                VGA::RAMDACData::Write(idxcol << 2);
                VGA::RAMDACData::Write(idxcol << 3);
            }
            break;
        default:
            for (int idxcol = 0; idxcol < 256; ++idxcol)
            {
                VGA::RAMDACData::Write(idxcol << 3);
                VGA::RAMDACData::Write(idxcol << 3);
                VGA::RAMDACData::Write(idxcol << 3);
            }
            break;
        }
    }
}

void InitializePalettes(ModeDescriptor& descriptor)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        VideoParameters& parameters = descriptor.GetParameters();
        AttributeControllerData::Write(AttributeController::Register::Palette0, parameters.AttributePalette, 16);
        AttributeController::BorderColor::Write(parameters.Config.BorderColor);
        InitializeRAMDACPalette(descriptor);
    }
}

Hag::VGA::Sequencer::ClockingMode_t ToggleScreenOnOff(Hag::VGA::Sequencer::ClockingMode_t screenToggle)
{
    VGA::Sequencer::ClockingMode_t originalClockingMode = VGA::Sequencer::ClockingMode::Read();

    screenToggle &= VGA::Sequencer::ClockingMode::ScreenOff;

    VGA::Sequencer::ClockingMode_t newClockingMode = (originalClockingMode & ~VGA::Sequencer::ClockingMode::ScreenOff) | screenToggle;

    VGA::Sequencer::ClockingMode::Write(newClockingMode);

    return originalClockingMode;
}

Hag::VGA::Sequencer::ClockingMode_t TurnScreenOn()
{
    return ToggleScreenOnOff(VGA::Sequencer::ClockingMode::ScreenOn);
}

Hag::VGA::Sequencer::ClockingMode_t TurnScreenOff()
{
    return ToggleScreenOnOff(VGA::Sequencer::ClockingMode::ScreenOff);
}

bool IsExtensionReg7Writeable()
{
    Shared::CRTCExtensionIndex::Write(0x07);
    Shared::CRTCExtensionData_t save = Shared::CRTCExtensionData::Read();
    Shared::CRTCExtensionData::Write(0x07, 0x0A);
    bool equal = Shared::CRTCExtensionData::Read() == 0x0A;
    Shared::CRTCExtensionData::Write(0x07, save);
    return equal;
}

uint8_t Data0x7814 = 0xEE;

void ResetCRTCExtensionRegisters()
{
    if (!IsExtensionReg7Writeable())
    {
        Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) &
            ~(Shared::Indexed::MiscellaneousControl::MAFCFuncSelect | Shared::Indexed::MiscellaneousControl::VGADACBitDepth));

        Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device, Shared::Indexed::MultiplexControl::Bits8p);

        Shared::PCI::Indexed::GeneralControl::Write(System::s_Device,
            (Shared::PCI::Indexed::GeneralControl::Read(System::s_Device) &
            ~Shared::Indexed::GeneralControl::PedestalControl) |
            ((Data0x7814 & 0x01) << 4));

        Shared::PCI::Indexed::GeneralControl::Write(System::s_Device,
            (Shared::PCI::Indexed::GeneralControl::Read(System::s_Device) &
            ~Shared::Indexed::GeneralControl::GreenChannelSync) |
            (Data0x7814 & Shared::Indexed::GeneralControl::GreenChannelSync));

        Shared::PCI::Indexed::GeneralControl::Write(System::s_Device,
            Shared::PCI::Indexed::GeneralControl::Read(System::s_Device) &
            (Shared::Indexed::GeneralControl::PedestalControl |
            Shared::Indexed::GeneralControl::GreenChannelSync));

        Shared::PCI::Indexed::CursorControl::Write(System::s_Device,
            Shared::Indexed::CursorControl::Disabled);

        Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
            (Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) &
            ~Shared::Indexed::PixelClockControl::ClockSelection) |
            Shared::Indexed::PixelClockControl::ClockPLL);
    }

    Shared::CRTCExtension::AddressGeneratorExtensions::Write(0x00);

    Shared::CRTCExtension::Miscellaneous::Write(
        (Data0x7814 & (Shared::CRTCExtension::Miscellaneous::CompositeSyncEnable >> 2)) << 2);

    Shared::CRTCExtension::MemoryPage::Write(0x00);
    Shared::CRTCExtension::HorizontalHalfCount::Write(0x00);
}


void SetupSquareWave()
{
    using namespace Hag::System;

    PIT::Command::Write(PIT::Command::ModeSquareWaveGenerator | PIT::Command::LowByteHighByte | PIT::Command::SelectChannel2);
    PIT::Data::WriteChannel2(0xA9);
    PIT::Data::WriteChannel2(0x04);
}

void Sleep(uint8_t count)//count * 8 * 1193 ticks time
{
    using namespace Hag::System;

    SetupSquareWave();

    KB::Register_t port = (BIOS::MachineID::Read() == 0xFC) ? KB::Register::PortB : KB::Register::PortXT;

    count <<= 3;

    if (count != 0)
    {
        KB::PortB_t orgValue = KB::PortBStatus::Read();
        KB::PortBCommand::Write(orgValue |= KB::PortBCommand::Timer2GateSpeakerEnable);

        do
        {
            volatile KB::PortB_t status;
            do
            {
                status = KB::Read(port);
            } while ((status & KB::PortBStatus::MirrorTimer2OutputCond) != 0);

            do
            {
                status = KB::Read(port);
            } while ((status & KB::PortBStatus::MirrorTimer2OutputCond) != 0);
            --count;
        } while (count != 0);

        KB::PortBCommand::Write(orgValue);
    }
}

void MiniSleep()//1193 ticks time
{
    using namespace Hag::System;

    SetupSquareWave();

    KB::Register_t port = (BIOS::MachineID::Read() == 0xFC) ? KB::Register::PortB : KB::Register::PortXT;

    KB::PortB_t orgValue = KB::PortBStatus::Read();
    KB::PortBCommand::Write(orgValue |= KB::PortBCommand::Timer2GateSpeakerEnable);

    volatile KB::PortB_t status;
    do
    {
        status = KB::Read(port);
    } while ((status & KB::PortBStatus::MirrorTimer2OutputCond) != 0);
    KB::PortBCommand::Write(orgValue);
}

void ApplyParameters(ModeDescriptor& descriptor, Hag::VGA::Register_t baseVideoIOPort)
{
    SYS_ClearInterrupts();
    
    VideoParameters& parameters = descriptor.GetParameters();

    VGA::Sequencer::Reset::Write(VGA::Sequencer::Reset::AsynchronousReset);
    VGA::SequencerData::Write(VGA::Sequencer::Register::ClockingMode, parameters.Config.Sequencer, sizeof(parameters.Config.Sequencer));
    VGA::MiscellaneousOutput::Write(parameters.GetMiscellaneousOutput());
    
    MiniSleep();

    VGA::Sequencer::Reset::Write(VGA::Sequencer::Reset::AsynchronousReset | VGA::Sequencer::Reset::SynchronousReset);

    SYS_RestoreInterrupts();

    VGA::CRTController::VerticalRetraceEnd::Write(baseVideoIOPort, 0x00);

    VGA::CRTController::HorizontalTotal::Write(baseVideoIOPort, parameters.Timings.Horizontal.Total);                           //CR00
    VGA::CRTController::HorizontalDisplayEnd::Write(baseVideoIOPort, parameters.Timings.Horizontal.DisplayEnd);                 //CR01
    VGA::CRTController::StartHorizontalBlank::Write(baseVideoIOPort, parameters.Timings.Horizontal.StartBlankLow);              //CR02
    VGA::CRTController::EndHorizontalBlank::Write(baseVideoIOPort, parameters.Timings.Horizontal.EndBlank);                     //CR03
    VGA::CRTController::StartHorizontalSyncPosition::Write(baseVideoIOPort, parameters.Timings.Horizontal.StartSyncPosition);   //CR04
    VGA::CRTController::EndHorizontalSyncPosition::Write(baseVideoIOPort, parameters.Timings.Horizontal.EndSyncPosition);       //CR05
    VGA::CRTController::VerticalTotal::Write(baseVideoIOPort, parameters.Timings.Vertical.Total);                               //CR06
    VGA::CRTController::CRTCOverflow::Write(baseVideoIOPort, parameters.Timings.Vertical.Overflow);                             //CR07
    VGA::CRTController::PresetRowScan::Write(baseVideoIOPort, parameters.Config.PresetRowScan);                                 //CR08
    VGA::CRTController::MaximumScanLine::Write(baseVideoIOPort, parameters.GetMaximumScanline());                               //CR09
    VGA::CRTController::CursorStartScanLine::Write(baseVideoIOPort, parameters.Font.CursorStartScanLine);                       //CR0A
    VGA::CRTController::CursorEndScanLine::Write(baseVideoIOPort, parameters.Font.CursorEndScanLine);                           //CR0B
    VGA::CRTController::StartAddressHigh::Write(baseVideoIOPort, 0);                                                            //CR0C
    VGA::CRTController::StartAddressLow::Write(baseVideoIOPort, 0);                                                             //CR0D
    VGA::CRTController::CursorLocationAddressHigh::Write(baseVideoIOPort, 0);                                                   //CR0E
    VGA::CRTController::CursorLocationAddressLow::Write(baseVideoIOPort, 0);                                                    //CR0F
    VGA::CRTController::VerticalRetraceStart::Write(baseVideoIOPort, parameters.Timings.Vertical.RetraceStart);                 //CR10
    VGA::CRTController::VerticalRetraceEnd::Write(baseVideoIOPort, parameters.GetVerticalRetraceEnd());                         //CR11
    VGA::CRTController::VerticalDisplayEnd::Write(baseVideoIOPort, parameters.Timings.Vertical.DisplayEnd);                     //CR12
    
    uint16_t offset = descriptor.CalculateOffset();
    VGA::CRTController::ScreenOffset::Write(baseVideoIOPort, uint8_t(offset));                                                  //CR13
    Shared::CRTCExtension::AddressGeneratorExtensions::Write(((offset >> 4) &
        Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8) |
        (Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
        ~Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8));

    VGA::CRTController::UnderlineLocation::Write(baseVideoIOPort, parameters.Config.UnderlineLocation);                         //CR14
    VGA::CRTController::StartVerticalBlank::Write(baseVideoIOPort, parameters.Timings.Vertical.StartBlank);                     //CR15
    VGA::CRTController::EndVerticalBlank::Write(baseVideoIOPort, parameters.Timings.Vertical.EndBlank);                         //CR16
    VGA::CRTController::CRTCModeControl::Write(baseVideoIOPort, parameters.Config.CRTCModeControl);                             //CR17
    VGA::CRTController::LineCompare::Write(baseVideoIOPort, parameters.Config.LineCompare);                                     //CR18

    VGA::InputStatus1::Read(baseVideoIOPort + 0x06);
    VGA::FeatureControl::Write(baseVideoIOPort + 0x06, 0x00);

    VGA::AttributeController::AttributeMode::Write(parameters.Config.AttributeMode);                                            //AR10
    VGA::AttributeController::ColorPlane::Write(parameters.Config.ColorPlane);                                                  //AR12
    VGA::AttributeController::HorizontalPixelPanning::Write(parameters.Config.HorizontalPixelPanning);                          //AR13

    VGA::GraphicsControllerData::Write(VGA::GraphicsController::Register::SetResetData, parameters.Config.Graphics, 9);
}

void ClearScreen(ModeDescriptor& descriptor)
{
    using namespace Hag::System;

    if ((BDA::VideoModeOptions::Get() & BDA::VideoModeOptions::DontClearDisplay) == 0)
    {
        uint16_t value = ((descriptor.Flags & Flags::Mode) == Flags::Text) ? 0x0720 : 0x0000;

        VGA::Sequencer::EnableWritePlane_t writePlane = VGA::Sequencer::EnableWritePlane::Read();
        if ((descriptor.Flags & Flags::MemoryOrganization) == Flags::Planar)
        {
            VGA::Sequencer::EnableWritePlane::Write(
                VGA::Sequencer::EnableWritePlane::Plane1 |
                VGA::Sequencer::EnableWritePlane::Plane2 |
                VGA::Sequencer::EnableWritePlane::Plane3 |
                VGA::Sequencer::EnableWritePlane::Plane4);
        }

        uint16_t pages = 1;
        if (((descriptor.Width * descriptor.Height * descriptor.Bpp) >> 3) > (64*1024))
        {
            pages = System::s_MemorySize >> 6;
        }

        uint16_t clearCount = 0x8000;
        if ((descriptor.Segment & 0xF000) == 0xB000)
            clearCount = 0x4000;

        for (uint16_t page = 0; page < pages; ++page)
        {
            Shared::CRTCExtension::MemoryPage::Write(page);
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
        Shared::CRTCExtension::MemoryPage::Write(0);

        if ((descriptor.Flags & Flags::MemoryOrganization) == Flags::Planar)
        {
            VGA::Sequencer::EnableWritePlane::Write(writePlane);
        }
    }
}

void ConfigureFontLoadMemoryMapping()
{
    VGA::Sequencer::EnableWritePlane::Write(VGA::Sequencer::EnableWritePlane::Plane3);
    Shared::Sequencer::MemoryModeControl::Write(
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::Unknown1 |
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::SequentialAddressingMode);
    
    VGA::GraphicsController::ReadPlaneSelect::Write(VGA::GraphicsController::ReadPlaneSelect::Plane3);
    VGA::GraphicsController::GraphicsControllerMode::Write(VGA::GraphicsController::GraphicsControllerMode::Mode0);
    VGA::GraphicsController::MemoryMapModeControl::Write(VGA::GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);
}

void ConfigureTextMemoryMapping()
{
    using namespace Hag::System;

    VGA::Sequencer::EnableWritePlane::Write(VGA::Sequencer::EnableWritePlane::Plane1 | VGA::Sequencer::EnableWritePlane::Plane2);
    Shared::Sequencer::MemoryModeControl::Write(
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::Unknown1 |
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::ExtendedMemoryAddress);

    VGA::GraphicsController::ReadPlaneSelect::Write(VGA::GraphicsController::ReadPlaneSelect::Plane1);
    VGA::GraphicsController::GraphicsControllerMode::Write(VGA::GraphicsController::GraphicsControllerMode::OddEvenAddressing);
    if (BDA::VideoBaseIOPort::Get() == VGA::Register::CRTControllerIndexD)
        VGA::GraphicsController::MemoryMapModeControl::Write(
            VGA::GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
            VGA::GraphicsController::MemoryMapModeControl::B8000HtoBFFFFH);
    else
        VGA::GraphicsController::MemoryMapModeControl::Write(
            VGA::GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
            VGA::GraphicsController::MemoryMapModeControl::B0000HtoB7FFFH);
}

void UploadFont(FontConfiguration& fontConfig)
{
    static uint8_t Bank[] = { 0x00, 0x40, 0x80, 0xC0, 0x20, 0x60, 0xA0, 0xE0 };

    uint16_t offset = 0;
    uint16_t count =fontConfig.CharacterCount + 1;
    uint8_t* font = fontConfig.Font;
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

void ApplyGraphicsCharacterSetOverride()
{
    using namespace Hag::System;
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
            InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = graphicsCharacterSet->CharacterFontDefinitionTable;
        }
    }
}

void SetInterruptTableFontPointer(ModeDescriptor& descriptor)
{
    using namespace Hag::System;
    if (GetNumberOfActiveScanlines(descriptor) != Scanlines::S200)
    {
        InterruptTable::Pointer<InterruptTable::CharacterTable>() = System::s_Font8x16;
    }
    BDA::CursorScanLines::Get().End = 0;
    BDA::CursorScanLines::Get().Start = 0;
    ApplyGraphicsCharacterSetOverride();
}

void ApplyPowerGraphicsSettings(ModeDescriptor& descriptor)
{
    using namespace Hag::System;

    if ((descriptor.Flags & Flags::MAFCSelection) == Flags::MAFCVGA)
    {
        Shared::PCI::Option::WriteByte1(System::s_Device,
            Shared::PCI::Option::ReadByte1(System::s_Device) &
            ~Shared::PCI::Option::SplitMode);

        Shared::CRTCExtension::MemoryPage::Write(0x00);

        if (descriptor.Bpp == BitsPerPixel::Bpp32)
        {
            BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2) | 0x03;

            Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) |
                Shared::Indexed::MiscellaneousControl::VGADAC8Bit);

            Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device,
                Shared::Indexed::MultiplexControl::Bits24p8x);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv4 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);
        }
        else if ((descriptor.Bpp == BitsPerPixel::Bpp15) ||
                 (descriptor.Bpp == BitsPerPixel::Bpp16))
        {
            BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2) | 0x02;

            Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) |
                Shared::Indexed::MiscellaneousControl::VGADAC8Bit);

            Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device,
                (descriptor.Bpp == BitsPerPixel::Bpp15) ?
                Shared::Indexed::MultiplexControl::Bits15p :
                Shared::Indexed::MultiplexControl::Bits16p);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv2 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);
        }
        else
        {
            BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2) | 0x01;

            Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) &
                ~Shared::Indexed::MiscellaneousControl::VGADACBitDepth);

            Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device,
                Shared::Indexed::MultiplexControl::Bits8p);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv1 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);
        }

        Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) |
            Shared::Indexed::MiscellaneousControl::MAFCDisable);

        Shared::CRTCExtension::HorizontalHalfCount::Write(1);
    }
    else
    {
        BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2);

        Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) &
            ~Shared::Indexed::MiscellaneousControl::MAFCFuncSelect);
    }
}

void SetStartAddress(uint32_t address)
{
    VGA::CRTController::StartAddressLow::Write(VGA::Register::CRTControllerIndexD, uint8_t(address));
    VGA::CRTController::StartAddressHigh::Write(VGA::Register::CRTControllerIndexD, uint8_t(address >> 8));
    Shared::CRTCExtension::AddressGeneratorExtensions::Write(
        (Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
        ~Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16) |
        (uint8_t(address >> 16) & Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16));
}

void ApplyMode(ModeDescriptor& descriptor, Hag::System::BDA::VideoModeOptions_t videoModeOptions)
{
    using namespace Hag::System;

    BDA::DisplayMode::Get() = descriptor.LegacyMode;
    BDA::VideoModeOptions::Get() = videoModeOptions;
    BDA::VideoBaseIOPort::Get() = descriptor.CrtController;

    ResetCRTCExtensionRegisters();

    BDA::ActiveDisplayNumber::Get() = 0;
    BDA::VideoBufferOffset::Get() = 0;
    BDA::CursorPositions::Clear();

    VideoParameters& parameters = descriptor.GetParameters();

    BDA::NumberOfScreenColumns::Get() = parameters.Config.NumCharacterColumns;
    BDA::RowsOnScreen::Get() = parameters.Config.NumScreenRowsMinus1;
    BDA::PointHeightOfCharacterMatrix::Get() = parameters.Config.CharacterLength;

    if (parameters.Timings.FrequencyKHz == 0)//Standard VGA
        BDA::VideoBufferSize::Get() = parameters.Config.VideoBufferSize;

    BDA::CursorScanLines::Get().Start = parameters.Font.CursorStartScanLine;
    BDA::CursorScanLines::Get().End = parameters.Font.CursorEndScanLine;

    ApplyParameters(descriptor, BDA::VideoBaseIOPort::Get());

    VGA::AttributeController::PixelPadding::Write(0);

    if (parameters.Timings.FrequencyKHz != 0)//Power Graphics mode
        ApplyPowerGraphicsSettings(descriptor);

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

    if (parameters.Timings.FrequencyKHz != 0)
        SetStartAddress(0);

    VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06);
    VGA::AttributeControllerIndex::Write(VGA::AttributeControllerIndex::EnableVideoDisplay);
    
    if (descriptor.CRTModeControlRegValue != 0xFF)
        BDA::CRTModeControlRegValue::Get() = descriptor.CRTModeControlRegValue;

    if (descriptor.CGAColorPaletteMaskSetting != 0xFF)
        BDA::CGAColorPaletteMaskSetting::Get() = descriptor.CGAColorPaletteMaskSetting;

    BDA::VideoDisplayDataArea::Get() &= ~BDA::VideoDisplayDataArea::Reserved;
}

//
//Calculate the M, N, P and S values for the PLL's given an input frequency in KHz
//
//inputs:
//   eax = requested frequency in KHz
//
//outputs:
//   ebx = M,N,P and S values
//         bl holds N in lower 7 bits
//         bh holds M in lower 5 bits
//         bh holds P in upper 3 bits
//         S 2 bits sit above bh
//         00000000 000000SS PPPMMMMM xNNNNNNN
//
uint32_t CalculatePLL_MNPS(uint32_t requestedFrequencyKHz)//Offset 0x5e4b
{
    requestedFrequencyKHz = min<uint32_t>(max<uint32_t>(requestedFrequencyKHz, 6262), 218864);
    
    uint32_t mnps = 0;
    uint32_t difference = 218864;
    uint32_t frequency = 220000;

    for (uint16_t N = 100; N < 128; ++N)
    {
        for (uint16_t M = 0; M < 32; ++M)
        {
            uint32_t approx = uint32_t((14318 * (N + 1)) / (M + 1));

            if (approx < 50000)
                continue;

            if (approx > 220000)
                continue;

            for (uint8_t P = 0; P < 4; ++P)
            {
                uint32_t attempt = approx / (1 << P);

                if (attempt < requestedFrequencyKHz)
                {
                    attempt = requestedFrequencyKHz - attempt;
                }
                else
                {
                    attempt = attempt - requestedFrequencyKHz;
                }

                if (attempt > difference)
                    continue;

                bool same = attempt == difference;

                difference = attempt;

                if (same && (approx >= frequency))
                    continue;

                frequency = approx;

                uint32_t S = 0;
                if (frequency <= 100000) S = 0;
                else if (frequency <= 140000) S = 1 << 16;
                else if (frequency <= 180000) S = 2 << 16;
                else S = 3 << 16;

                mnps = N | (M << 8);
                mnps |= ((1 << P) - 1) << 13;
                mnps |= S;
            }
        }
    }
    return mnps;
}

typedef uint8_t PixelClocksSettings_t;
namespace PixelClocksSettings
{
    enum
    {
        PLLSet = 0x03,
        PLLSetA = 0x00,
        PLLSetB = 0x01,
        PLLSetC = 0x02,
        Clock = 0xC,
        ClockPCI = 0x00,
        ClockPLL = 0x04,
        ClockVDOCLK = 0x08
    };
}

void ConfigurePixelClocks(uint32_t mnps, PixelClocksSettings_t PllAndClock)
{
    //5.7.8.3 (A) Step 1: Force the screen off.
    VGA::Sequencer::ClockingMode::Write(
        VGA::Sequencer::ClockingMode::Read() |
        VGA::Sequencer::ClockingMode::ScreenOff);

    //5.7.8.3 (A) Step 2: Set pixclkdis to '1' (disable the pixel clock and video clocks)
    Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
        Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) |
        Shared::Indexed::PixelClockControl::ClockDisable);

    VGA::DACWriteIndex_t pllIndex = Shared::Indexed::Register::PixelPLLMA;

    if ((PllAndClock & PixelClocksSettings::PLLSet) == PixelClocksSettings::PLLSetB)
        pllIndex = Shared::Indexed::Register::PixelPLLMB;
    else if ((PllAndClock & PixelClocksSettings::PLLSet) == PixelClocksSettings::PLLSetC)
        pllIndex = Shared::Indexed::Register::PixelPLLMC;

    //5.7.8.3 (A) Step 3: Re-program the desired pixel PLL registers.
    Shared::PCI::IndexedData::Write(System::s_Device, pllIndex + 0, (mnps >> 8) & Shared::Indexed::PixelPLLM::MValue);
    Shared::PCI::IndexedData::Write(System::s_Device, pllIndex + 1, mnps & Shared::Indexed::PixelPLLN::NValue);
    Shared::PCI::IndexedData::Write(System::s_Device, pllIndex + 2, (mnps  >> 13) &
        (Shared::Indexed::PixelPLLP::PValue | Shared::Indexed::PixelPLLP::SValue));

    Shared::Indexed::PixelClockControl_t clockControl = Shared::Indexed::PixelClockControl::ClockPCI;

    if ((PllAndClock & PixelClocksSettings::Clock) == PixelClocksSettings::ClockPLL)
        clockControl = Shared::Indexed::PixelClockControl::ClockPLL;
    else if ((PllAndClock & PixelClocksSettings::Clock) == PixelClocksSettings::ClockVDOCLK)
        clockControl = Shared::Indexed::PixelClockControl::ClockVDCLK;

    //5.7.8.3 (A) Step 3 (continued): Select another source for the pixel clock.
    Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
        (Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) &
        ~Shared::Indexed::PixelClockControl::ClockSelection) |
        clockControl);

    //5.7.8.3 (A) Step 4: Wait until the clock source is locked onto its new frequency (the pixlock bit is '1')
    //                    for the pixel PLL, or for the VDCLK pin to become stable.
    SYS_ClearInterrupts();
    Sleep(1);
    SYS_RestoreInterrupts();

    volatile Shared::Indexed::PixelPLLStatus_t frequencyLock = Shared::Indexed::PixelPLLStatus::FrequencyNotLocked;
    uint16_t timeout = 0;
    do
    {
        --timeout;
        if (timeout == 0)
            break;

        frequencyLock = Shared::PCI::Indexed::PixelPLLStatus::Read(System::s_Device) &
            Shared::Indexed::PixelPLLStatus::FrequencyStatus;
    } while (frequencyLock == Shared::Indexed::PixelPLLStatus::FrequencyNotLocked);

    //5.7.8.3 (A) Step 5: Set pixclkdis to '0' (enable the pixel and video clocks)
    Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
        Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) &
        ~Shared::Indexed::PixelClockControl::ClockDisable);

    VGA::Sequencer::ClockingMode::Write(VGA::Sequencer::ClockingMode::Read() & ~VGA::Sequencer::ClockingMode::ScreenOff);
}

SetVideoError_t SetVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate, bool clearDisplay)
{
    using namespace Hag::System;

    SetVideoError_t error = SetVideoError::Success;
    ModeDescriptor* descriptor = GetModeDescriptor(width, height, bpp, flags, refreshRate, error);
    if (error != SetVideoError::Success)
        return error;

    VideoParameters& parameters = descriptor->GetParameters();

    TurnScreenOff();
    VGA::MiscellaneousOutput::Write(~VGA::MiscellaneousOutput::VideoEnable);
    //Turn monitor off
    CRTCExtension::HorizontalCounterExtensions::Write(
        parameters.Timings.HorizontalCounterExtensions |
        CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
        CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff);
    Shared::CRTCExtension::VerticalCounterExtensions::Write(parameters.Timings.VerticalCounterExtensions);

    if (parameters.Timings.FrequencyKHz != 0)
        ConfigurePixelClocks(CalculatePLL_MNPS(parameters.Timings.FrequencyKHz), PixelClocksSettings::PLLSetC | PixelClocksSettings::ClockPLL);

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

    InterruptTable::Pointer<InterruptTable::CharacterTable>() = System::s_Font8x8;
    InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = System::s_Font8x8Graphics;

    videoModeOptions &= ~(BDA::VideoModeOptions::Unknown | BDA::VideoModeOptions::Inactive);

    ApplyMode(*descriptor, videoModeOptions);

    //Turn monitor on
    CRTCExtension::HorizontalCounterExtensions::Write(
        CRTCExtension::HorizontalCounterExtensions::Read() &
        ~(CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
        CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff));
    TurnScreenOn();

    return SetVideoError::Success;
}

}}}}}