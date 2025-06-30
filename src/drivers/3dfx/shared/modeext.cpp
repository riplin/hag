//Copyright 2025-Present riplin

#include <dos.h>
#include <stdio.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/extmsapi.h>
#include "modintl.h"
#include "sysintl.h"

#include <hag/drivers/3dfx/shared/io/viddsa.h>
#include <hag/drivers/3dfx/shared/io/dacmode.h>
#include <hag/drivers/3dfx/shared/io/drminit1.h>
#include <hag/drivers/3dfx/shared/io/vidscrs.h>
#include <hag/drivers/3dfx/shared/io/pllctrl0.h>
#include <hag/drivers/3dfx/shared/io/vgainit0.h>
#include <hag/drivers/3dfx/shared/io/vgainit1.h>
#include <hag/drivers/3dfx/shared/io/viddostr.h>
#include <hag/drivers/3dfx/shared/io/vidprcfg.h>
#include <hag/drivers/3dfx/shared/pci/ctbaddr.h>
#include <hag/drivers/3dfx/shared/pci/fbbaddr.h>

#include <hag/drivers/3dfx/shared/mmio2d/clip.h>
#include <hag/drivers/3dfx/shared/mmio2d/dstfmt.h>

namespace Hag::VGA::ModeSetting::External
{

bool Initialize(IAllocator& allocator)
{
    return TDfx::Shared::Function::System::Initialize(allocator);
}

void Shutdown()
{
    TDfx::Shared::Function::System::Shutdown();
}

static uint32_t CalculatePixelTotal(const ModeDescriptor& descriptor)
{
    const VideoParameters& table = descriptor.GetParameters();

    uint8_t dotClockSelect = table.Config.Sequencer[VGA::Sequencer::Register::ClockingMode - 1] & VGA::Sequencer::ClockingMode::DotClockSelect;
    uint8_t charWidth = dotClockSelect == 0 ? 9 : 8;

    uint32_t horizontalTotal = table.Timings.Horizontal.Total;

    uint8_t scanlineDouble = (table.Config.CRTCModeControl & VGA::CRTController::CRTCModeControl::VerticalTotalDouble) >> 2;

    uint32_t verticalTotal = table.Timings.Vertical.Total;
    //Extension bit 8 is in bit 0.
    uint32_t verticalTotalBit8 = table.Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalTotalHigh1;
    //Move to bit 8.
    verticalTotalBit8 <<= 8;
    //Move bit in to place.
    verticalTotal |= verticalTotalBit8;
    //Extension bit 9 is in bit 5.
    uint32_t verticalTotalBit9 = table.Timings.Vertical.Overflow & VGA::CRTController::CRTCOverflow::VerticalTotalHigh2;
    //Move to bit 9.
    verticalTotalBit9 <<= 4;
    //Move bit in to place.
    verticalTotal |= verticalTotalBit9;
 
    if ((descriptor.Flags & TDfx::Shared::Function::ModeSetting::Flags::ModeType) ==
        TDfx::Shared::Function::ModeSetting::Flags::TDfx)
    {
        TDfx::Shared::Function::ModeSetting::ResolutionTimings& timings =
        *(TDfx::Shared::Function::ModeSetting::ResolutionTimings*)&table.Timings;

        //Extension bit 8 is in bit 0.
        uint32_t horizontalTotalBit8 = timings.HorizontalExtensions & TDfx::Shared::CRTController::HorizontalExtension::HorizontalTotal8;
        //Move to bit 8.
        horizontalTotalBit8 <<= 8;
        //Move bit in to place.
        horizontalTotal |= horizontalTotalBit8;

        //Extension bit 10 is in bit 0.
        uint32_t verticalTotalBit10 = timings.VerticalExtensions & TDfx::Shared::CRTController::VerticalExtension::VerticalTotal10;
        //Move to bit 10.
        verticalTotalBit10 <<= 10;
        //Move bit in to place.
        verticalTotal |= verticalTotalBit10;
    }

    horizontalTotal += 5;
    horizontalTotal *= charWidth;

    if (table.Config.Sequencer[VGA::Sequencer::Register::ClockingMode - 1] & VGA::Sequencer::ClockingMode::InternalCharacterClock)
        horizontalTotal <<= 1;

    verticalTotal += 2;
    verticalTotal <<= scanlineDouble;
    return verticalTotal * horizontalTotal;
}

static uint32_t CalculatePLLFrequency(uint8_t N, uint8_t M, uint8_t K)
{
    return uint32_t(((uint64_t(14318180) * (N + 2)) / (M + 2)) / (1 << K));
}

static uint32_t CalcPLLFreq(uint16_t pll)
{
    using namespace Hag::TDfx::Shared;
    uint8_t N = uint8_t((pll & PLLControl0::NMultiplier) >> PLLControl0::Shift::NMultiplier);
    uint8_t M = uint8_t((pll & PLLControl0::MInputDivider) >> PLLControl0::Shift::MInputDivider);
    uint8_t K = uint8_t((pll & PLLControl0::KPostDivider) >> PLLControl0::Shift::KPostDivider);
    return CalculatePLLFrequency(N, M, K);
}

uint32_t PLLFromFrequency(uint32_t frequency)
{
    using namespace Hag::TDfx::Shared;

    uint32_t freq = frequency;
    uint32_t error = 0xffffffff;
    uint16_t CandidateKPostDivider = 0;
    uint16_t CandidateNMultiplier = 0;
    uint16_t CandidateMInputDivider = 0;

    for (uint16_t KPostDivider = 0; KPostDivider < 4; ++KPostDivider)
    {
        for (uint16_t MInputDivider = 0; MInputDivider < 0x40; ++MInputDivider)
        {
            for (uint16_t NMultiplier = 0; NMultiplier < 253; ++NMultiplier)
            {
                freq = CalculatePLLFrequency(NMultiplier, MInputDivider, KPostDivider);

                uint32_t newError = freq - frequency;

                if (freq < frequency)
                    newError = -newError;

                if (error >= newError)
                {
                    error = newError;
                    CandidateKPostDivider = KPostDivider;
                    CandidateNMultiplier = NMultiplier;
                    CandidateMInputDivider = MInputDivider;
                }
            }
        }
    }

    return (CandidateKPostDivider << PLLControl0::Shift::KPostDivider) |
           (CandidateMInputDivider << PLLControl0::Shift::MInputDivider) |
           (CandidateNMultiplier << PLLControl0::Shift::NMultiplier);
}

static uint32_t CalculateFrequency(const ModeDescriptor& descriptor, RefreshRate_t refreshRate)
{
    uint32_t frequency = 0;
    if (descriptor.RefreshRate != refreshRate)
    {
        uint32_t pixelTotal = CalculatePixelTotal(descriptor);
        frequency = CalcPLLFreq(PLLFromFrequency(refreshRate * pixelTotal));
    }
    else
    {
        if ((descriptor.Flags & TDfx::Shared::Function::ModeSetting::Flags::ModeType) ==
            TDfx::Shared::Function::ModeSetting::Flags::TDfx)
        {
            TDfx::Shared::Function::ModeSetting::ResolutionTimings& timings =
            *(TDfx::Shared::Function::ModeSetting::ResolutionTimings*)&descriptor.GetParameters().Timings;

            frequency = CalcPLLFreq(timings.PLLControl0);
        }
    }
    return frequency;
}

SetVideoError_t SupportsRefreshRate(const ModeDescriptor& descriptor, RefreshRate_t refreshRate)
{
    // TODO monitor filter.
    if ((descriptor.Flags & TDfx::Shared::Function::ModeSetting::Flags::TDfx) != 0)
    {
        uint32_t frequency = CalculateFrequency(descriptor, refreshRate);
        printf("Frequency %ix%ix%ibpp@%iHz: %lu\n", descriptor.Width, descriptor.Height, descriptor.Bpp, refreshRate, frequency);
        return (frequency < 250000000) ? SetVideoError::Success : SetVideoError::NotSupportedByRamdac;
    }
    else
    {
        return descriptor.RefreshRate == refreshRate ? SetVideoError::Success : SetVideoError::RefreshRateNotSupported;
    }
}

bool IsExtendedMode(const ModeDescriptor& descriptor)
{
    return (descriptor.Flags & TDfx::Shared::Function::ModeSetting::Flags::ModeType) ==
        TDfx::Shared::Function::ModeSetting::Flags::TDfx;
}

void IterateModeDescriptors(const DescriptorCallback_t& callback)
{
    if (VGA::Data::IterateModeDescriptors(callback))
    {
        SetVideoError_t error = SetVideoError::Success;
        for (uint32_t i = 0; i < TDfx::Shared::Data::s_NumDescriptors; ++i)
        {
            error = SetVideoError::Success;

            ModeDescriptor& mode = TDfx::Shared::Data::s_Descriptors[i];

            uint32_t requiredMemory = mode.Stride * mode.Height;
            if (requiredMemory > (TDfx::Shared::Function::System::s_MemorySize << 10))
            {
                error = SetVideoError::InsufficientVideoMemory;
            }

            if (!callback(TDfx::Shared::Data::s_Descriptors[i], error))
                break;
        }
    }
}

void SetupRAMDAC(const ModeDescriptor& descriptor)
{
    //Nothing to set up.
}

void SetExtendedOffset(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;
    using namespace Hag::TDfx::Shared;
    
    const TDfx::Shared::Function::ModeSetting::ModeDescriptor& descr =
        *(const TDfx::Shared::Function::ModeSetting::ModeDescriptor*)&descriptor;

    CRTController::ScreenOffset::Write(BDA::VideoBaseIOPort::Get(), descr.Offset);
}

uint16_t GetNumberOf64KBPages()
{
    return TDfx::Shared::Function::System::s_MemorySize >> 6;
}

void SelectPage(uint16_t page)
{
    using namespace Hag::TDfx::Shared;
    page <<= 1;
    IO::VGAInit1::Write(Function::System::s_IOBaseAddress,
        (IO::VGAInit1::Read(Function::System::s_IOBaseAddress) &
        ~(VGAInit1::VBEReadAperture32K | VGAInit1::VBEWriteAperture32K)) |
        page | (page << VGAInit1::Shift::VBEReadAperture32K));
}

void DisableExtendedMode()
{
    using namespace Hag::TDfx::Shared;

    IO::VideoProcessorConfiguration::Write(Function::System::s_IOBaseAddress,
        IO::VideoProcessorConfiguration::Read(Function::System::s_IOBaseAddress) &
        ~VideoProcessorConfiguration::Enable);

    IO::VGAInit0::Write(Function::System::s_IOBaseAddress,
        IO::VGAInit0::Read(Function::System::s_IOBaseAddress) & VGAInit0::WakeUpSelect);

    IO::VGAInit1::Write(Function::System::s_IOBaseAddress,
        IO::VGAInit1::Read(Function::System::s_IOBaseAddress) & ~VGAInit1::EnableChain4Mode);
}

void ApplyExtendedModeSettings(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;
    using namespace Hag::TDfx::Shared;

    if ((descriptor.Flags & TDfx::Shared::Function::ModeSetting::Flags::ModeType) ==
        TDfx::Shared::Function::ModeSetting::Flags::TDfx)
    {
        BDA::VideoBufferSize::Get() = descriptor.GetParameters().Config.VideoBufferSize;

        const TDfx::Shared::Function::ModeSetting::ResolutionTimings& timings =
            *(const TDfx::Shared::Function::ModeSetting::ResolutionTimings*)&descriptor.GetParameters().Timings;

        IO::VGAInit0::Write(Function::System::s_IOBaseAddress,
                            IO::VGAInit0::Read(Function::System::s_IOBaseAddress) | VGAInit0::ExtensionsEnabled);
        IO::VGAInit0::Write(Function::System::s_IOBaseAddress,
                            (IO::VGAInit0::Read(Function::System::s_IOBaseAddress) & VGAInit0::WakeUpSelect) | VGAInit0::ExtensionsEnabled);

        System::PCI::Write16(Function::System::s_Device, System::PCI::PreHeader::Command,
        System::PCI::Read16(Function::System::s_Device, System::PCI::PreHeader::Command) & ~System::PCI::Command::VGAPaletteSnoop);

        IO::DACMode::Write(Function::System::s_IOBaseAddress, 0);
        IO::VideoDesktopStartAddress::Write(Function::System::s_IOBaseAddress, 0);
        IO::VideoScreenSize::Write(Function::System::s_IOBaseAddress,
            descriptor.Height << (((timings.Config & Function::ModeSetting::Configuration::HalfMode) != 0) ? 13 : 12) |
            (descriptor.Width & VideoScreenSize::Width));

        IO::VideoDesktopOverlayStride::Write(Function::System::s_IOBaseAddress, descriptor.Stride);

        IO::VGAInit0::Write(Function::System::s_IOBaseAddress,
            (IO::VGAInit0::Read(Function::System::s_IOBaseAddress) & VGAInit0::WakeUpSelect) |
            (uint32_t(timings.Config & Function::ModeSetting::Configuration::ExtendedVideoShiftOut) << 12) |
            VGAInit0::ExtensionsEnabled);

        IO::VGAInit1::Write(Function::System::s_IOBaseAddress,
            uint32_t(timings.Config & Function::ModeSetting::Configuration::ExtendedVideoShiftOut) << 20);

        VideoProcessorConfiguration_t videoProcessorConfiguration = 0;

        switch(descriptor.Bpp)
        {
        case BitsPerPixel::Bpp8:
            videoProcessorConfiguration = VideoProcessorConfiguration::DesktopPixelFormat8BitPalettized;
            break;
        case BitsPerPixel::Bpp16:
            videoProcessorConfiguration = VideoProcessorConfiguration::DesktopPixelFormat565Undithered;
            break;
        case BitsPerPixel::Bpp24:
            videoProcessorConfiguration = VideoProcessorConfiguration::DesktopPixelFormatRGB24Packed;
            break;
        case BitsPerPixel::Bpp32:
            videoProcessorConfiguration = VideoProcessorConfiguration::DesktopPixelFormatRGB32;
        }

        uint8_t* baseAddress2d = TDfx::Shared::PCI::ControlBaseAddress::GetBaseAddressAs<uint8_t>(Function::System::s_Device);

        if ((timings.Config & Function::ModeSetting::Configuration::ExtendedVideoShiftOut) != 0)
        {
            videoProcessorConfiguration |= VideoProcessorConfiguration::Enable |
                                        VideoProcessorConfiguration::DesktopEnable;

            if (descriptor.Bpp != BitsPerPixel::Bpp8)
                videoProcessorConfiguration |= VideoProcessorConfiguration::DesktopCLUTDisable |
                                            VideoProcessorConfiguration::OverlayCLUTDisable;

            videoProcessorConfiguration |=  (timings.Config & (Function::ModeSetting::Configuration::Interlace |
                                                            Function::ModeSetting::Configuration::HalfMode)) << 2;
            
            TwoD::DestinationFormat_t bpp2D = 0;
            switch(descriptor.Bpp)
            {
            case BitsPerPixel::Bpp8:
                bpp2D = TwoD::DestinationFormat::Bpp8;
                break;
            case BitsPerPixel::Bpp16:
                bpp2D = TwoD::DestinationFormat::Bpp16;
                break;
            case BitsPerPixel::Bpp24:
                bpp2D = TwoD::DestinationFormat::Bpp24;
                break;
            case BitsPerPixel::Bpp32:
                bpp2D = TwoD::DestinationFormat::Bpp32;
                break;
            default:
                break;
            }
            
            MMIO2D::DestinationFormat::Write(baseAddress2d, descriptor.Stride | bpp2D);

            MMIO2D::Clip::WriteClip0Min(baseAddress2d, (uint32_t(0) << TwoD::Clip::Shift::Y) | 0);
            MMIO2D::Clip::WriteClip0Max(baseAddress2d, (uint32_t(descriptor.Height) << TwoD::Clip::Shift::Y) | descriptor.Width);
            MMIO2D::Clip::WriteClip1Min(baseAddress2d, (uint32_t(0) << TwoD::Clip::Shift::Y) | 0);
            MMIO2D::Clip::WriteClip1Max(baseAddress2d, (uint32_t(descriptor.Height) << TwoD::Clip::Shift::Y) | descriptor.Width);

            //TODO: the 3D core probably needs this setup as well.
        }

        IO::VideoProcessorConfiguration::Write(Function::System::s_IOBaseAddress, videoProcessorConfiguration);

        IO::DRAMInit1::Write(Function::System::s_IOBaseAddress,
            IO::DRAMInit1::Read(Function::System::s_IOBaseAddress) |
            DRAMInit1::SGRAMRefreshEnable);
    }
}

void SetStartAddress(uint32_t startAddress)
{
    using namespace Hag::TDfx::Shared;

    IO::VideoDesktopStartAddress::Write(Function::System::s_IOBaseAddress, startAddress);
}

void TurnMonitorOff()
{

}

void TurnMonitorOn()
{

}

void WriteExtensionRegisters(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;
    using namespace Hag::TDfx;

    if ((descriptor.Flags & TDfx::Shared::Function::ModeSetting::Flags::ModeType) ==
        TDfx::Shared::Function::ModeSetting::Flags::TDfx)
    {
         const Shared::Function::ModeSetting::ResolutionTimings& timings =
            *(const Shared::Function::ModeSetting::ResolutionTimings*)&descriptor.GetParameters().Timings;
        
        Shared::CRTController::HorizontalExtension::Write(BDA::VideoBaseIOPort::Get(), timings.HorizontalExtensions);
        Shared::CRTController::VerticalExtension::Write(BDA::VideoBaseIOPort::Get(), timings.VerticalExtensions);
    }
    else
    {
        Shared::CRTController::HorizontalExtension::Write(BDA::VideoBaseIOPort::Get(), 0);
        Shared::CRTController::VerticalExtension::Write(BDA::VideoBaseIOPort::Get(), 0);
    }
}

uint16_t CalculatePLLControl(const ModeDescriptor& descriptor, RefreshRate_t refreshRate)
{
    using namespace Hag::TDfx::Shared;
    uint16_t pllControl = 0;    
    if (descriptor.RefreshRate == refreshRate)
    {
        if ((descriptor.Flags & Function::ModeSetting::Flags::ModeType) ==
            Function::ModeSetting::Flags::TDfx)
        {
            const Function::ModeSetting::ResolutionTimings& timings =
                *(const Function::ModeSetting::ResolutionTimings*)&descriptor.GetParameters().Timings;
            
            pllControl = timings.PLLControl0;
        }
    } else
    {
        uint32_t pixelTotal = CalculatePixelTotal(descriptor);
        pllControl = PLLFromFrequency(refreshRate * pixelTotal);
    }
    return pllControl;
}

void SetupClock(const ModeDescriptor& descriptor, RefreshRate_t refreshRate)
{
    using namespace Hag::TDfx::Shared;

    if ((descriptor.Flags & Function::ModeSetting::Flags::ModeType) ==
        Function::ModeSetting::Flags::TDfx)
    {
        uint16_t pllControl0 = CalculatePLLControl(descriptor, refreshRate);
        if (pllControl0 != 0)
            IO::PLLControl0::Write(Function::System::s_IOBaseAddress, pllControl0);
    }
}

void* GetLinearFrameBuffer()
{
    return TDfx::Shared::PCI::FrameBufferBaseAddress::GetBaseAddressAs<void>(TDfx::Shared::Function::System::s_Device);
}

}



/*

//Stride calculation...

        uint16_t stride = 0;
        if ((descriptor.Flags & Flags::Mode) == Flags::Text)
        {
            stride = descriptor.Width << 1;
        }
        else
        {
            VGA::ModeSetting::BitsPerPixel_t bpp = descriptor.Bpp;
            if (bpp == BitsPerPixel::Bpp15)
                bpp = BitsPerPixel::Bpp16;
            
            stride = (descriptor.Width * bpp) >> 3;
            if (bpp == BitsPerPixel::Bpp4)
                stride >>= 2;
            if (bpp == BitsPerPixel::Bpp24)
            {
                //Power of 2
                stride = (descriptor.Width * bpp) >> 3;
                uint8_t shift = 0;
                uint8_t bits = 0;
                while (stride != 0)
                {
                    bits += stride & 1;
                    stride >>= 1;
                    ++shift;
                }
                stride = 1 << (shift - 1);
                if (bits > 1)
                    stride <<= 1;
            }
        }


*/