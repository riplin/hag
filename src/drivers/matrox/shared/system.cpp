//Copyright 2025-Present riplin

#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <hag/system/pci.h>
#include <hag/system/pit.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/extmsapi.h>
#include <hag/drivers/matrox/shared/pci/fbap.h>
#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/pci/iloadap.h>
#include <hag/drivers/matrox/shared/funcs/system.h>
#include <hag/drivers/matrox/shared/crtcext/misc.h>         //CER3
#include <hag/drivers/matrox/shared/crtcext/mempg.h>        //CER4

namespace Hag::Matrox::Shared::Function::System
{

    
bool s_Initialized = false;
static IAllocator* s_Allocator = nullptr;
Hag::System::PCI::Device_t s_Device = 0xFFFF;
uint32_t s_MemorySize = 0;//Memory size in KB

uint8_t GetMemoryIn64KBlocks()
{
    bool valNotEqual = false;

    VGA::Sequencer::Register_t sequencerIndex = VGA::SequencerIndex::Read();
    VGA::Sequencer::ClockingMode_t clockingMode = VGA::Sequencer::ClockingMode::Read();

    VGA::Sequencer::ClockingMode::Write(clockingMode | VGA::Sequencer::ClockingMode::ScreenOff);

    Hag::System::PIT::MiniSleep();

    VGA::GraphicsController::MemoryMapModeControl_t memoryMapModeControl = VGA::GraphicsController::MemoryMapModeControl::Read();

    VGA::GraphicsController::MemoryMapModeControl::Write((memoryMapModeControl &
        ~VGA::GraphicsController::MemoryMapModeControl::MemoryMapMode) |
        VGA::GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);

    uint16_t timeout = 0;
    VGA::InputStatus1_t inputStatus = 0;

    do
    {
        --timeout;
        if (timeout == 0)
            break;

        inputStatus = VGA::InputStatus1::Read(VGA::Register::InputStatus1D) &
                                            VGA::InputStatus1::VerticalSync;
    } while (inputStatus == VGA::InputStatus1::VerticalSyncInactive);

    //Not sure why this is done twice...
    do
    {
        --timeout;
        if (timeout == 0)
            break;

        inputStatus = VGA::InputStatus1::Read(VGA::Register::InputStatus1D) &
                                            VGA::InputStatus1::VerticalSync;
    } while (inputStatus == VGA::InputStatus1::VerticalSyncInactive);

    Shared::CRTCExtension::Miscellaneous_t miscellaneous = Shared::CRTCExtension::Miscellaneous::Read();

    Shared::CRTCExtension::Miscellaneous::Write(Shared::CRTCExtension::Miscellaneous::ScaleDiv2 |
                                                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);

    Shared::CRTCExtension::MemoryPage_t memoryPage = Shared::CRTCExtension::MemoryPage::Read();

    uint8_t* upper = FARPointer(0xA000, 0xFFFE).ToPointer<uint8_t>();//No size, we only poke one byte.
    uint8_t* lower = FARPointer(0xA000, 0x0000).ToPointer<uint8_t>();//No size, we only poke one byte.

    uint16_t foundMem = 0x2000;
    uint16_t maxMem = 0x8000;//8MB max TODO: This should be different for other cards.
    uint8_t testPage = 0x20;

    do
    {
        Shared::CRTCExtension::MemoryPage::Write(testPage);

        uint8_t saveValue1 = *upper;
        *upper = 0xa5;

        uint8_t saveValue2 = *lower;
        *lower = 0x5a;

        valNotEqual = 0xa5 != *upper;

        *lower = saveValue2;
        *upper = saveValue1;

        if (valNotEqual)
            break;

        foundMem += 0x1000;
        testPage += 0x10;//Next MB

    } while (foundMem != maxMem);

    Shared::CRTCExtension::MemoryPage::Write(memoryPage);
    Shared::CRTCExtension::Miscellaneous::Write(miscellaneous);
    VGA::GraphicsController::MemoryMapModeControl::Write(memoryMapModeControl);
    VGA::Sequencer::ClockingMode::Write(clockingMode);
    VGA::SequencerIndex::Write(sequencerIndex);

    return foundMem >> 8;
}

bool Initialize(IAllocator& allocator)
{
    using namespace Hag::System;
    
    if (!s_Initialized)
    {
        s_Allocator = &allocator;

        if (!Hag::System::PCI::FindDevice(0x102B, 0x051A, s_Device))
            return false;

        s_MemorySize = GetMemoryIn64KBlocks() << 6;
            
        s_Initialized = true;

        s_Initialized &= VGA::ModeSetting::DeclareAperture(PCI::ControlAperture::Read(s_Device) &
                                                            PCI::ControlAperture::BaseAddress,
                                                            16 * 1024);

        s_Initialized &= VGA::ModeSetting::DeclareAperture(PCI::FrameBufferAperture::Read(s_Device) &
                                                            PCI::FrameBufferAperture::BaseAddress,
                                                            s_MemorySize << 10);

        s_Initialized &= VGA::ModeSetting::DeclareAperture(PCI::ILOADAperture::Read(s_Device) &
                                                            PCI::ILOADAperture::BaseAddress,
                                                            8 * 1024 * 1024);
    }
    return s_Initialized;
}

void Shutdown()
{
    //Nothing to do. Stay initialized.
}

uint32_t GetMemorySize()
{
    uint32_t ret = 0;
    if (s_Initialized)
        ret = s_MemorySize << 10;

    return ret;
}

}