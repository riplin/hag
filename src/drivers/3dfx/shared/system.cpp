//Copyright 2025-Present riplin

#include <dos.h>
#include <dpmi.h>
#include <hag/system/pci.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/vga/extmsapi.h>

#include <hag/drivers/3dfx/shared/pci/iobaddr.h>
#include <hag/drivers/3dfx/shared/pci/fbbaddr.h>
#include <hag/drivers/3dfx/shared/io/drminit0.h>

namespace Hag::TDfx::Shared::Function::System
{

bool s_Initialized = false;
static IAllocator* s_Allocator = nullptr;
Hag::System::PCI::Device_t s_Device = 0xFFFF;
uint16_t s_IOBaseAddress = 0xFFFF;
uint32_t s_MemorySize = 0;//Memory size in KB

uint16_t GetMemoryIn64KBlocks()
{
    using namespace Hag::TDfx;
    static uint8_t MemorySizes[] ={ 0x10, 0x20, 0x20, 0x40 };
    uint32_t index = (Shared::IO::DRAMInit0::Read(s_IOBaseAddress) &
                      (Shared::DRAMInit0::SGRAMChipSetCount |
                       Shared::DRAMInit0::SGRAMChipSize)) >>
                      Shared::DRAMInit0::Shift::SGRAMChipSetCount;

    return uint16_t(MemorySizes[index]) << 2;
}

bool Initialize(IAllocator& allocator)
{
    using namespace Hag::System;
    
    if (!s_Initialized)
    {
        s_Allocator = &allocator;

        Hag::System::PCI::FindDevice(0x121a, 0x0003, s_Device);
        // if (!Hag::System::PCI::FindDevice(0x121a, 0x0003, s_Device))
        //     return false;

        s_IOBaseAddress = Shared::PCI::IOBaseAddress::GetBaseAddress(s_Device);
        
        s_MemorySize = GetMemoryIn64KBlocks() << 6;
        s_Initialized = VGA::ModeSetting::DeclareAperture(PCI::FrameBufferBaseAddress::Read(s_Device) &
                                                            PCI::FrameBufferBaseAddress::Address,
                                                            s_MemorySize << 10);
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