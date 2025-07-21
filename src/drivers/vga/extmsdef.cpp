//Copyright 2025-Present riplin

#include <dos.h>
#include <hag/drivers/vga/extmsapi.h>

//Default implementation for stock VGA modes.

namespace Hag::VGA::ModeSetting::External
{

bool Initialize(Has::IAllocator& allocator)
{
    return true;
}

void Shutdown()
{

}

SetVideoError_t SupportsRefreshRate(const ModeDescriptor& descriptor, RefreshRate_t refreshRate)
{
    return (descriptor.RefreshRate == refreshRate) ? SetVideoError::Success : SetVideoError::RefreshRateNotSupported;
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

void SetupClock(const ModeDescriptor& descriptor, RefreshRate_t refreshRate)
{

}

void* GetLinearFrameBuffer()
{
    return nullptr;
}

SetVideoError_t SetupBuffers(Buffers_t buffers)
{
    //TODO: properly implement this. There are some modes that can do double buffer.
    if ((buffers & Buffers::DepthBuffer) != 0)
        return SetVideoError::DepthBufferNotSupported;
    return (buffers != Buffers::SingleBuffer) ? SetVideoError::InsufficientVideoMemory : SetVideoError::Success;
}

void SwapScreen2D(bool waitForVSync)
{
    //TODO
}

}