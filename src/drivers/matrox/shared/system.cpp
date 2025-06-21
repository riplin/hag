//Copyright 2025-Present riplin

#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <hag/system/pci.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/matrox/shared/data.h>
#include <hag/drivers/matrox/shared/funcs/system.h>

#include <hag/drivers/matrox/shared/crtcext/misc.h>         //CER3
#include <hag/drivers/matrox/shared/crtcext/mempg.h>        //CER4

namespace Hag::Matrox::Shared::Function
{

namespace System
{

static uint16_t s_FontSelector = 0;

bool s_Initialized = false;
Hag::System::PCI::Device_t s_Device = 0xFFFF;
uint32_t s_MemorySize = 0;//Memory size in KB
FARPointer s_Font8x8;
FARPointer s_Font8x8Graphics;
FARPointer s_Font8x16;
FARPointer s_SystemFont;
FARPointer s_SystemFontGraphics;

uint8_t GetMemoryIn64KBlocks()
{
    bool valNotEqual = false;

    VGA::Sequencer::Register_t sequencerIndex = VGA::SequencerIndex::Read();
    VGA::Sequencer::ClockingMode_t clockingMode = VGA::Sequencer::ClockingMode::Read();

    VGA::Sequencer::ClockingMode::Write(clockingMode | VGA::Sequencer::ClockingMode::ScreenOff);

    //TODO: Speed sensitive!!!!!
    for (uint16_t i = 0x4b0; i != 0x0000; --i);

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

bool Initialize()
{
    if (!s_Initialized)
    {
        uint16_t size = Data::Font8x8Size + Data::Font8x16Size;
    
        Hag::System::PCI::FindDevice(0x102B, 0x051A, s_Device);
        // if (!Hag::System::PCI::FindDevice(0x102B, 0x051A, s_Device))
        //     return false;

        REGS r;
        memset(&r, 0, sizeof(r));

        r.w.ax = 0x0100;
        r.w.bx = (size + 15) >> 4;
        int386(0x31, &r, &r);

        if ((r.w.flags & 0x01) == 0)
        {
            uint32_t fontSegment = r.w.ax;
            s_FontSelector = r.w.dx;

            s_Font8x8 = FARPointer(fontSegment, 0x0000);
            s_Font8x8Graphics = FARPointer(fontSegment, Data::Font8x8Size >> 1);
            s_Font8x16 = FARPointer(fontSegment, Data::Font8x8Size);
            memcpy(s_Font8x8.ToPointer<uint8_t>(Data::Font8x8Size), Data::Font8x8, Data::Font8x8Size);
            memcpy(s_Font8x16.ToPointer<uint8_t>(Data::Font8x16Size), Data::Font8x16, Data::Font8x16Size);

            s_SystemFont = Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::CharacterTable>();
            s_SystemFontGraphics = Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::GraphicsFont8x8>();

            s_MemorySize = GetMemoryIn64KBlocks() << 6;

            s_Initialized = true;
        }
    }
    return s_Initialized;
}

void Shutdown()
{
    if (s_Initialized)
    {
        Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::CharacterTable>() = s_SystemFont;
        Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::GraphicsFont8x8>() = s_SystemFontGraphics;

        REGS r;
        memset(&r, 0, sizeof(r));
        r.w.ax = 0x0101;
        r.w.dx = s_FontSelector;
        int386(0x31, &r, &r);
        s_Initialized = false;
    }
}

uint32_t GetMemorySize()
{
    uint32_t ret = 0;
    if (s_Initialized)
    {
        ret = s_MemorySize << 10;
    }
    return ret;
}

}

}