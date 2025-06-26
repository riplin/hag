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
#include <hag/drivers/3dfx/shared/pci/fbbaddr.h>

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

            BitsPerPixel_t bpp = mode.Bpp;
            if (bpp == BitsPerPixel::Bpp15)
                bpp = BitsPerPixel::Bpp16;
            uint32_t requiredMemory = (mode.Width * mode.Height * bpp) >> 3;
            if (requiredMemory > (TDfx::Shared::Function::System::s_MemorySize << 10))
            {
                error = SetVideoError::InsufficientVideoMemory;
            }
            else
            {
                const TDfx::Shared::Function::ModeSetting::ResolutionTimings& timings =
                    *(const TDfx::Shared::Function::ModeSetting::ResolutionTimings*)&mode.GetParameters().Timings;

                (void)timings;//TODO
            }
            //TODO monitor filter.

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

        IO::VideoDesktopOverlayStride::Write(Function::System::s_IOBaseAddress, stride);

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

        if ((timings.Config & Function::ModeSetting::Configuration::ExtendedVideoShiftOut) != 0)
        {
            videoProcessorConfiguration |= VideoProcessorConfiguration::Enable |
                                        VideoProcessorConfiguration::DesktopEnable;

            if (descriptor.Bpp != BitsPerPixel::Bpp8)
                videoProcessorConfiguration |= VideoProcessorConfiguration::DesktopCLUTDisable |
                                            VideoProcessorConfiguration::OverlayCLUTDisable;

            videoProcessorConfiguration |=  (timings.Config & (Function::ModeSetting::Configuration::Interlace |
                                                            Function::ModeSetting::Configuration::HalfMode)) << 2;
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

void SetupClock(const ModeDescriptor& descriptor)
{
    using namespace Hag::TDfx::Shared;

    if ((descriptor.Flags & Function::ModeSetting::Flags::ModeType) ==
        Function::ModeSetting::Flags::TDfx)
    {
         const Function::ModeSetting::ResolutionTimings& timings =
            *(const Function::ModeSetting::ResolutionTimings*)&descriptor.GetParameters().Timings;

        if (timings.PLLControl0 != 0)
            IO::PLLControl0::Write(Function::System::s_IOBaseAddress, timings.PLLControl0);
    }
}

void* GetLinearFrameBuffer()
{
    return TDfx::Shared::PCI::FrameBufferBaseAddress::GetBaseAddressAs<void>(TDfx::Shared::Function::System::s_Device);
}

}