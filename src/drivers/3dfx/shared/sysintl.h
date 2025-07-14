//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>
#include <has/farptr.h>
#include <has/system/pci.h>
#include <hag/drivers/vga/extmsapi.h>

namespace Hag::TDfx::Shared::Function::System
{

extern bool s_Initialized;
extern Has::System::PCI::Device_t s_Device;
extern uint16_t s_IOBaseAddress;
extern uint8_t* s_LinearFrameBuffer;
extern uint8_t* s_ControlAperture;
extern uint32_t s_MemorySize;//Memory size in KB

extern uint8_t s_SurfaceCount;
extern uint8_t s_FrontSurface;
extern uint8_t s_BackSurface;
extern uint32_t s_Surfaces[3];

extern uint32_t s_DepthSurface;
extern uint32_t s_TextureMemory;
extern uint32_t s_TextureMemoryEnd;

extern bool Initialize(Has::IAllocator& allocator);
extern void Shutdown();

extern VGA::ModeSetting::SetVideoError_t SetupBuffers(VGA::ModeSetting::Buffers_t buffers);
extern void SwapScreen2D(bool waitForVSync);
extern void CleanUpBuffers();

}