//Copyright 2025-Present riplin

#include <dos.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/extmsapi.h>

//Default implementation for stock VGA modes.

namespace Hag::VGA::Data
{
    extern ModeSetting::ModeDescriptor s_Descriptors[];
    extern uint16_t s_NumDescriptors;
    extern uint8_t Font8x8[];
    extern uint16_t Font8x8Size;
    extern uint8_t Font8x16[];
    extern uint16_t Font8x16Size;
}

namespace Hag::VGA::ModeSetting::External
{

static uint16_t s_FontSelector = 0;

bool s_Initialized = false;

FARPointer s_Font8x8;
FARPointer s_Font8x8Graphics;
FARPointer s_Font8x16;
FARPointer s_SystemFont;
FARPointer s_SystemFontGraphics;

bool Initialize()
{
    if (!s_Initialized)
    {
        uint16_t size = Data::Font8x8Size + Data::Font8x16Size;

        REGS r;
        memset(&r, 0, sizeof(r));

        r.w.ax = 0x0100;
        r.w.bx = (size + 15) >> 4;
        int386(0x31, &r, &r);

        if ((r.w.flags & 0x01) == 0)
        {

            uint16_t fontSegment = r.w.ax;
            s_FontSelector = r.w.dx;

            s_Font8x8 = FARPointer(fontSegment, 0x0000);
            s_Font8x8Graphics = FARPointer(fontSegment, Data::Font8x8Size >> 1);
            s_Font8x16 = FARPointer(fontSegment, Data::Font8x8Size);
            memcpy(s_Font8x8.ToPointer<uint8_t>(Data::Font8x8Size), Data::Font8x8, Data::Font8x8Size);
            memcpy(s_Font8x16.ToPointer<uint8_t>(Data::Font8x16Size), Data::Font8x16, Data::Font8x16Size);

            SYS_ClearInterrupts();
            s_SystemFont = Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::CharacterTable>();
            s_SystemFontGraphics = Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::GraphicsFont8x8>();
            SYS_RestoreInterrupts();

            s_Initialized = true;
        }
    }
    return s_Initialized;
}

void Shutdown()
{
    if (s_Initialized)
    {
        SYS_ClearInterrupts();
        Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::CharacterTable>() = s_SystemFont;
        Hag::System::InterruptTable::Pointer<Hag::System::InterruptTable::GraphicsFont8x8>() = s_SystemFontGraphics;
        SYS_RestoreInterrupts();

        REGS r;
        memset(&r, 0, sizeof(r));
        r.w.ax = 0x0101;
        r.w.dx = s_FontSelector;
        int386(0x31, &r, &r);
        s_Initialized = false;
    }
}

bool IsExtendedMode(const ModeDescriptor& descriptor)
{
    return false;
}

void IterateModeDescriptors(const DescriptorCallback_t& callback)
{
    Data::IterateModeDescriptors(callback);
}

void SetupRAMDAC(const ModeDescriptor& descriptor)
{

}

void SetExtendedOffset(const ModeDescriptor& descriptor)
{

}

uint16_t GetNumberOf64KBPages()
{
    return 1;
}

void SelectPage(uint16_t page)
{

}

void UploadFont(const FontConfiguration& fontConfig)
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

const FARPointer& Get8x8Font()
{
    return s_Font8x8;
}

const FARPointer& Get8x8GraphicsFont()
{
    return s_Font8x8Graphics;
}

const FARPointer& Get8x16Font()
{
    return s_Font8x16;
}

void DisableExtendedMode()
{

}

void ApplyExtendedModeSettings(const ModeDescriptor& descriptor)
{

}

void SetStartAddress(uint32_t startAddress)
{
    VGA::CRTController::StartAddressLow::Write(VGA::Register::CRTControllerIndexD, uint8_t(startAddress));
    VGA::CRTController::StartAddressHigh::Write(VGA::Register::CRTControllerIndexD, uint8_t(startAddress >> 8));
}

void TurnMonitorOff()
{

}

void TurnMonitorOn()
{

}

void WriteExtensionRegisters(const ModeDescriptor& descriptor)
{

}

void SetupClock(const ModeDescriptor& descriptor)
{

}

}