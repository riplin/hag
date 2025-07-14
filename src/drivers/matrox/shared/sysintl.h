//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>
#include <has/farptr.h>
#include <has/ialloc.h>
#include <has/system/pci.h>

namespace Hag::Matrox::Shared::Function::System
{

extern bool s_Initialized;
extern Has::System::PCI::Device_t s_Device;
extern uint32_t s_MemorySize;//Memory size in KB

extern bool Initialize(Has::IAllocator& allocator);
extern void Shutdown();

}