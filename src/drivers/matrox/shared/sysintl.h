//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/farptr.h>
#include <hag/ialloc.h>
#include <hag/system/pci.h>

namespace Hag::Matrox::Shared::Function::System
{

extern bool s_Initialized;
extern Hag::System::PCI::Device_t s_Device;
extern uint32_t s_MemorySize;//Memory size in KB

extern bool Initialize(IAllocator& allocator);
extern void Shutdown();

}