//Copyright 2025-Present riplin

#include <dos.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/extmsapi.h>

//Default implementation for stock VGA modes.

namespace Hag::VGA::ModeSetting::External
{

bool Initialize(IAllocator& allocator)
{
    return true;
}

void Shutdown()
{

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

void DisableExtendedMode()
{

}

void ApplyExtendedModeSettings(const ModeDescriptor& descriptor)
{

}

void SetStartAddress(uint32_t startAddress)
{

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

void* GetLinearFrameBuffer()
{
    return nullptr;
}

}