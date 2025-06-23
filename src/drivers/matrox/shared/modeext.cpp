//Copyright 2025-Present riplin

#include <hag/system/bda.h>
#include <hag/system/pit.h>
#include <hag/drivers/vga/extmsapi.h>
#include <hag/drivers/matrox/shared/funcs/system.h>
#include <hag/drivers/matrox/shared/crtcext/adgenext.h>
#include <hag/drivers/matrox/shared/crtcext/mempg.h>
#include <hag/drivers/matrox/shared/crtcext/misc.h>
#include <hag/drivers/matrox/shared/crtcext/hrhlfcnt.h>
#include <hag/drivers/matrox/shared/pci/opt.h>
#include <hag/drivers/matrox/shared/pci/idx/miscctrl.h>
#include <hag/drivers/matrox/shared/pci/idx/muxctrl.h>
#include <hag/drivers/matrox/shared/pci/idx/genctrl.h>
#include <hag/drivers/matrox/shared/pci/idx/pclkctrl.h>
#include <hag/drivers/matrox/shared/pci/idx/curctrl.h>
#include <hag/drivers/matrox/shared/pci/idx/pixpllm.h>
#include <hag/drivers/matrox/shared/pci/idx/pixplln.h>
#include <hag/drivers/matrox/shared/pci/idx/pixpllp.h>
#include <hag/drivers/matrox/shared/pci/idx/pixpllst.h>
#include "modintl.h"
#include "sysintl.h"

namespace Hag::VGA::ModeSetting::External
{

bool Initialize(IAllocator& allocator)
{
    return Matrox::Shared::Function::System::Initialize(allocator);
}

void Shutdown()
{
    Matrox::Shared::Function::System::Shutdown();
}

bool IsExtendedMode(const ModeDescriptor& descriptor)
{
    if ((descriptor.Flags & Matrox::Shared::Function::ModeSetting::Flags::ModeType) ==
        Matrox::Shared::Function::ModeSetting::Flags::Matrox)
    {
        Matrox::Shared::Function::ModeSetting::ResolutionTimings& timings =
            *(Matrox::Shared::Function::ModeSetting::ResolutionTimings*)&descriptor.Parameters[0]->Timings;
        return timings.FrequencyKHz != 0;
    }
    return false;
}

void IterateModeDescriptors(const DescriptorCallback_t& callback)
{
    if (VGA::Data::IterateModeDescriptors(callback))
    {
        SetVideoError_t error = SetVideoError::Success;
        for (uint32_t i = 0; i < Matrox::Shared::Data::s_NumDescriptors; ++i)
        {
            error = SetVideoError::Success;

            ModeDescriptor& mode = Matrox::Shared::Data::s_Descriptors[i];

            uint32_t requiredMemory = (mode.Width * mode.Height * mode.Bpp) >> 3;
            if (requiredMemory > (Matrox::Shared::Function::System::s_MemorySize << 10))
            {
                error = SetVideoError::InsufficientVideoMemory;
            }
            else
            {
                Matrox::Shared::Function::ModeSetting::ResolutionTimings& timings =
                    *(Matrox::Shared::Function::ModeSetting::ResolutionTimings*)&mode.Parameters[0]->Timings;

                if (timings.FrequencyKHz >= 220000)//TODO: we should have this number live somewhere.
                    error = SetVideoError::NotSupportedByRamdac;
            }
            //TODO monitor filter.

            if (!callback(Matrox::Shared::Data::s_Descriptors[i], error))
                break;
        }
    }
}

void SetupRAMDAC(const ModeDescriptor& descriptor)
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

void SetExtendedOffset(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;

    if ((descriptor.Flags & Matrox::Shared::Function::ModeSetting::Flags::ModeType) == Matrox::Shared::Function::ModeSetting::Flags::Matrox)
    {
        uint16_t offset = 0;
        const VGA::ModeSetting::VideoParameters& parameters = descriptor.GetParameters();
        const Matrox::Shared::Function::ModeSetting::ResolutionTimings& timings = *(const Matrox::Shared::Function::ModeSetting::ResolutionTimings*)&parameters.Timings;

        if ((timings.FrequencyKHz != 0) &&
            ((descriptor.Flags & Flags::Mode) != Flags::Text) &&
            (descriptor.Bpp >= BitsPerPixel::Bpp8))
        {
            BitsPerPixel_t bpp = descriptor.Bpp;
            if (bpp == BitsPerPixel::Bpp15)
                bpp = BitsPerPixel::Bpp16;
                
            uint8_t dotClockSelect = ((parameters.Config.Sequencer[VGA::Sequencer::Register::ClockingMode - 1] &
                                       VGA::Sequencer::ClockingMode::DotClockSelect) == 0) ? 9 : 8;

            offset = uint16_t(((timings.Horizontal.DisplayEnd + 1) * dotClockSelect * bpp) / 128);
        }
        else
        {
            offset = descriptor.CalculateVGAOffset();
        }

        VGA::CRTController::ScreenOffset::Write(BDA::VideoBaseIOPort::Get(), uint8_t(offset));
        Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::Write(((offset >> 4) &
            Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8) |
            (Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
            ~Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8));
    }
}

uint16_t GetNumberOf64KBPages()
{
    return Matrox::Shared::Function::System::s_MemorySize >> 6;
}

void SelectPage(uint16_t page)
{
    Matrox::Shared::CRTCExtension::MemoryPage::Write(page);
}

bool IsExtensionReg7Writeable()
{
    using namespace Hag::Matrox;

    Shared::CRTCExtensionIndex::Write(0x07);
    Shared::CRTCExtensionData_t save = Shared::CRTCExtensionData::Read();
    Shared::CRTCExtensionData::Write(0x07, 0x0A);
    bool equal = Shared::CRTCExtensionData::Read() == 0x0A;
    Shared::CRTCExtensionData::Write(0x07, save);
    return equal;
}

uint8_t Data0x7814 = 0xEE;

void DisableExtendedMode()
{
    using namespace Hag::Matrox;

    if (!IsExtensionReg7Writeable())
    {
        Shared::PCI::Indexed::MiscellaneousControl::Write(Shared::Function::System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(Shared::Function::System::s_Device) &
            ~(Shared::Indexed::MiscellaneousControl::MAFCFuncSelect | Shared::Indexed::MiscellaneousControl::VGADACBitDepth));

        Shared::PCI::Indexed::MultiplexControl::Write(Shared::Function::System::s_Device, Shared::Indexed::MultiplexControl::Bits8p);

        Shared::PCI::Indexed::GeneralControl::Write(Shared::Function::System::s_Device,
            (Shared::PCI::Indexed::GeneralControl::Read(Shared::Function::System::s_Device) &
            ~Shared::Indexed::GeneralControl::PedestalControl) |
            ((Data0x7814 & 0x01) << 4));

        Shared::PCI::Indexed::GeneralControl::Write(Shared::Function::System::s_Device,
            (Shared::PCI::Indexed::GeneralControl::Read(Shared::Function::System::s_Device) &
            ~Shared::Indexed::GeneralControl::GreenChannelSync) |
            (Data0x7814 & Shared::Indexed::GeneralControl::GreenChannelSync));

        Shared::PCI::Indexed::GeneralControl::Write(Shared::Function::System::s_Device,
            Shared::PCI::Indexed::GeneralControl::Read(Shared::Function::System::s_Device) &
            (Shared::Indexed::GeneralControl::PedestalControl |
            Shared::Indexed::GeneralControl::GreenChannelSync));

        Shared::PCI::Indexed::CursorControl::Write(Shared::Function::System::s_Device,
            Shared::Indexed::CursorControl::Disabled);

        Shared::PCI::Indexed::PixelClockControl::Write(Shared::Function::System::s_Device,
            (Shared::PCI::Indexed::PixelClockControl::Read(Shared::Function::System::s_Device) &
            ~Shared::Indexed::PixelClockControl::ClockSelection) |
            Shared::Indexed::PixelClockControl::ClockPLL);
    }

    Shared::CRTCExtension::AddressGeneratorExtensions::Write(0x00);

    Shared::CRTCExtension::Miscellaneous::Write(
        (Data0x7814 & (Shared::CRTCExtension::Miscellaneous::CompositeSyncEnable >> 2)) << 2);

    Shared::CRTCExtension::MemoryPage::Write(0x00);
    Shared::CRTCExtension::HorizontalHalfCount::Write(0x00);
}

void ApplyExtendedModeSettings(const ModeDescriptor& descriptor)
{
    using namespace Hag::System;
    using namespace Hag::Matrox;

    if ((descriptor.Flags & Shared::Function::ModeSetting::Flags::MAFCSelection) == Shared::Function::ModeSetting::Flags::MAFCVGA)
    {
        Shared::PCI::Option::WriteByte1(Shared::Function::System::s_Device,
            Shared::PCI::Option::ReadByte1(Shared::Function::System::s_Device) &
            ~Shared::PCI::Option::SplitMode);

        Shared::CRTCExtension::MemoryPage::Write(0x00);

        if (descriptor.Bpp == BitsPerPixel::Bpp32)
        {
            BDA::VideoBufferSize::Get() = (Shared::Function::System::s_MemorySize << 2) | 0x03;

            Shared::PCI::Indexed::MiscellaneousControl::Write(Shared::Function::System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(Shared::Function::System::s_Device) |
                Shared::Indexed::MiscellaneousControl::VGADAC8Bit);

            Shared::PCI::Indexed::MultiplexControl::Write(Shared::Function::System::s_Device,
                Shared::Indexed::MultiplexControl::Bits24p8x);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv4 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);
        }
        else if ((descriptor.Bpp == BitsPerPixel::Bpp15) ||
                 (descriptor.Bpp == BitsPerPixel::Bpp16))
        {
            BDA::VideoBufferSize::Get() = (Shared::Function::System::s_MemorySize << 2) | 0x02;

            Shared::PCI::Indexed::MiscellaneousControl::Write(Shared::Function::System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(Shared::Function::System::s_Device) |
                Shared::Indexed::MiscellaneousControl::VGADAC8Bit);

            Shared::PCI::Indexed::MultiplexControl::Write(Shared::Function::System::s_Device,
                (descriptor.Bpp == BitsPerPixel::Bpp15) ?
                Shared::Indexed::MultiplexControl::Bits15p :
                Shared::Indexed::MultiplexControl::Bits16p);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv2 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);
        }
        else
        {
            BDA::VideoBufferSize::Get() = (Shared::Function::System::s_MemorySize << 2) | 0x01;

            Shared::PCI::Indexed::MiscellaneousControl::Write(Shared::Function::System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(Shared::Function::System::s_Device) &
                ~Shared::Indexed::MiscellaneousControl::VGADACBitDepth);

            Shared::PCI::Indexed::MultiplexControl::Write(Shared::Function::System::s_Device,
                Shared::Indexed::MultiplexControl::Bits8p);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv1 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);
        }

        Shared::PCI::Indexed::MiscellaneousControl::Write(Shared::Function::System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(Shared::Function::System::s_Device) |
            Shared::Indexed::MiscellaneousControl::MAFCDisable);

        Shared::CRTCExtension::HorizontalHalfCount::Write(1);
    }
    else
    {
        BDA::VideoBufferSize::Get() = (Shared::Function::System::s_MemorySize << 2);

        Shared::PCI::Indexed::MiscellaneousControl::Write(Shared::Function::System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(Shared::Function::System::s_Device) &
            ~Shared::Indexed::MiscellaneousControl::MAFCFuncSelect);
    }
}

void SetStartAddress(uint32_t startAddress)
{
    VGA::CRTController::StartAddressLow::Write(VGA::Register::CRTControllerIndexD, uint8_t(startAddress));
    VGA::CRTController::StartAddressHigh::Write(VGA::Register::CRTControllerIndexD, uint8_t(startAddress >> 8));
    Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::Write(
        (Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
        ~Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16) |
        (uint8_t(startAddress >> 16) & Matrox::Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16));
}

void TurnMonitorOff()
{
    Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Write(
        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Read() |
        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff);
}

void TurnMonitorOn()
{
    Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Write(
        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Read() &
        ~(Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff));
}

void WriteExtensionRegisters(const ModeDescriptor& descriptor)
{
    if ((descriptor.Flags & Matrox::Shared::Function::ModeSetting::Flags::ModeType) ==
        Matrox::Shared::Function::ModeSetting::Flags::Matrox)
    {
        Matrox::Shared::Function::ModeSetting::ResolutionTimings& timings =
            *(Matrox::Shared::Function::ModeSetting::ResolutionTimings*)&descriptor.Parameters[0]->Timings;

        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Write(
            timings.HorizontalCounterExtensions |
            (Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Read() &
            (Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
            Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff)));
        Matrox::Shared::CRTCExtension::VerticalCounterExtensions::Write(timings.VerticalCounterExtensions);
    }
    else
    {
        Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Write(
            (Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::Read() &
            (Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
            Matrox::Shared::CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff)));
        Matrox::Shared::CRTCExtension::VerticalCounterExtensions::Write(0);
    }
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
    using namespace Hag::System;
    using namespace Hag::Matrox;

    //5.7.8.3 (A) Step 1: Force the screen off.
    // Screen is already off.
    // VGA::Sequencer::ClockingMode::Write(
    //     VGA::Sequencer::ClockingMode::Read() |
    //     VGA::Sequencer::ClockingMode::ScreenOff);

    //5.7.8.3 (A) Step 2: Set pixclkdis to '1' (disable the pixel clock and video clocks)
    Shared::PCI::Indexed::PixelClockControl::Write(Shared::Function::System::s_Device,
        Shared::PCI::Indexed::PixelClockControl::Read(Shared::Function::System::s_Device) |
        Shared::Indexed::PixelClockControl::ClockDisable);

    VGA::DACWriteIndex_t pllIndex = Shared::Indexed::Register::PixelPLLMA;

    if ((PllAndClock & PixelClocksSettings::PLLSet) == PixelClocksSettings::PLLSetB)
        pllIndex = Shared::Indexed::Register::PixelPLLMB;
    else if ((PllAndClock & PixelClocksSettings::PLLSet) == PixelClocksSettings::PLLSetC)
        pllIndex = Shared::Indexed::Register::PixelPLLMC;

    //5.7.8.3 (A) Step 3: Re-program the desired pixel PLL registers.
    Shared::PCI::IndexedData::Write(Shared::Function::System::s_Device, pllIndex + 0, (mnps >> 8) & Shared::Indexed::PixelPLLM::MValue);
    Shared::PCI::IndexedData::Write(Shared::Function::System::s_Device, pllIndex + 1, mnps & Shared::Indexed::PixelPLLN::NValue);
    Shared::PCI::IndexedData::Write(Shared::Function::System::s_Device, pllIndex + 2, (mnps  >> 13) &
        (Shared::Indexed::PixelPLLP::PValue | Shared::Indexed::PixelPLLP::SValue));

    Shared::Indexed::PixelClockControl_t clockControl = Shared::Indexed::PixelClockControl::ClockPCI;

    if ((PllAndClock & PixelClocksSettings::Clock) == PixelClocksSettings::ClockPLL)
        clockControl = Shared::Indexed::PixelClockControl::ClockPLL;
    else if ((PllAndClock & PixelClocksSettings::Clock) == PixelClocksSettings::ClockVDOCLK)
        clockControl = Shared::Indexed::PixelClockControl::ClockVDCLK;

    //5.7.8.3 (A) Step 4: Wait until the clock source is locked onto its new frequency (the pixlock bit is '1')
    //                    for the pixel PLL, or for the VDCLK pin to become stable.
    SYS_ClearInterrupts();
    PIT::Sleep(1);
    SYS_RestoreInterrupts();

    volatile Shared::Indexed::PixelPLLStatus_t frequencyLock = Shared::Indexed::PixelPLLStatus::FrequencyNotLocked;
    uint16_t timeout = 0;
    do
    {
        --timeout;
        if (timeout == 0)
            break;

        frequencyLock = Shared::PCI::Indexed::PixelPLLStatus::Read(Shared::Function::System::s_Device) &
            Shared::Indexed::PixelPLLStatus::FrequencyStatus;
    } while (frequencyLock == Shared::Indexed::PixelPLLStatus::FrequencyNotLocked);

    //This step is moved (in contrast to the docs). Wait for the clock to stabilize before selecting it as source.
    //5.7.8.3 (A) Step 3 (continued): Select another source for the pixel clock.
    Shared::PCI::Indexed::PixelClockControl::Write(Shared::Function::System::s_Device,
        (Shared::PCI::Indexed::PixelClockControl::Read(Shared::Function::System::s_Device) &
        ~Shared::Indexed::PixelClockControl::ClockSelection) |
        clockControl);

    //5.7.8.3 (A) Step 5: Set pixclkdis to '0' (enable the pixel and video clocks)
    Shared::PCI::Indexed::PixelClockControl::Write(Shared::Function::System::s_Device,
        Shared::PCI::Indexed::PixelClockControl::Read(Shared::Function::System::s_Device) &
        ~Shared::Indexed::PixelClockControl::ClockDisable);
}

void SetupClock(const ModeDescriptor& descriptor)
{
    if ((descriptor.Flags & Matrox::Shared::Function::ModeSetting::Flags::ModeType) ==
        Matrox::Shared::Function::ModeSetting::Flags::Matrox)
    {
        Matrox::Shared::Function::ModeSetting::ResolutionTimings& timings =
            *(Matrox::Shared::Function::ModeSetting::ResolutionTimings*)&descriptor.Parameters[0]->Timings;

        if (timings.FrequencyKHz != 0)
            ConfigurePixelClocks(CalculatePLL_MNPS(timings.FrequencyKHz), PixelClocksSettings::PLLSetC | PixelClocksSettings::ClockPLL);
    }
}

}