//Copyright 2025-Present riplin

#include <dos.h>
#include <dpmi.h>
#include <has/system/pci.h>
#include <has/system/interrup.h>
#include <hag/drivers/vga/vga.h>

#include <hag/drivers/3dfx/shared/io/viddsa.h>
#include <hag/drivers/3dfx/shared/pci/ctbaddr.h>
#include <hag/drivers/3dfx/shared/pci/iobaddr.h>
#include <hag/drivers/3dfx/shared/pci/fbbaddr.h>
#include <hag/drivers/3dfx/shared/io/drminit0.h>
#include <hag/drivers/3dfx/shared/io/drminit1.h>
#include <hag/drivers/3dfx/shared/fifo/handler.h>
#include <hag/drivers/3dfx/shared/fifo/pck2.h>

#include <hag/drivers/3dfx/shared/mmio2d/baseaddr.h>
#include <hag/drivers/3dfx/shared/mmio3d/buffaddr.h>

#include "sysintl.h"
#include "modintl.h"

namespace Hag::TDfx::Shared::Function::System
{

bool s_Initialized = false;
static Has::IAllocator* s_Allocator = nullptr;
Has::System::PCI::Device_t s_Device = 0xFFFF;
uint16_t s_IOBaseAddress = 0xFFFF;
uint8_t* s_LinearFrameBuffer = nullptr;
uint8_t* s_ControlAperture = nullptr;
uint32_t s_MemorySize = 0;//Memory size in KB

uint8_t s_SurfaceCount = 0;
uint8_t s_FrontSurface = 0;
uint8_t s_BackSurface = 0;
uint32_t s_Surfaces[3] = { 0 };

uint32_t s_DepthSurface = 0;
uint32_t s_TextureMemory = 0;
uint32_t s_TextureMemoryEnd = 0;

Fifo::Handler<0> s_FifoHandler0;
Fifo::Handler<1> s_FifoHandler1;

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

bool Initialize(Has::IAllocator& allocator)
{
    using namespace Hag::System;

    if (!s_Initialized)
    {
        s_Allocator = &allocator;

        Has::System::PCI::FindDevice(0x121a, 0x0003, s_Device);
        // if (!Has::System::PCI::FindDevice(0x121a, 0x0003, s_Device))
        //     return false;

        s_IOBaseAddress = Shared::PCI::IOBaseAddress::GetBaseAddress(s_Device);

        s_MemorySize = GetMemoryIn64KBlocks() << 6;
        s_Initialized = VGA::ModeSetting::DeclareAperture(PCI::FrameBufferBaseAddress::Read(s_Device) &
                                                          PCI::FrameBufferBaseAddress::Address,
                                                          s_MemorySize << 10);
        s_Initialized = VGA::ModeSetting::DeclareAperture(PCI::ControlBaseAddress::Read(s_Device) &
                                                          PCI::ControlBaseAddress::Address,
                                                          6 * 1024 * 1024);

        s_LinearFrameBuffer = Shared::PCI::FrameBufferBaseAddress::GetBaseAddressAs<uint8_t>(s_Device);
        s_ControlAperture = Shared::PCI::ControlBaseAddress::GetBaseAddressAs<uint8_t>(s_Device);

        Fifo::CommandFifoFetchThreshhold::Write(s_ControlAperture, 
            (9 << Fifo::CommandFifoFetchThreshhold::Shift::HighWaterMark) |
            (2 << Fifo::CommandFifoFetchThreshhold::Shift::LowWaterMark));
    }
    return s_Initialized;
}

static void SetupFifos(uint32_t basePage4KFifo0, uint32_t basePage4KFifo1, uint8_t pageCount)
{
    s_FifoHandler0.Setup(s_ControlAperture, s_LinearFrameBuffer, basePage4KFifo0, pageCount);
    s_FifoHandler1.Setup(s_ControlAperture, s_LinearFrameBuffer, basePage4KFifo1, pageCount);
}

static void ShutdownFifos()
{
    s_FifoHandler0.Shutdown();
    s_FifoHandler1.Shutdown();
}

void Shutdown()
{
    CleanUpBuffers();
}

uint32_t GetMemorySize()
{
    uint32_t ret = 0;
    if (s_Initialized)
        ret = s_MemorySize << 10;

    return ret;
}

static void ApplyBufferSettings()
{
    IO::VideoDesktopStartAddress::Write(s_IOBaseAddress, s_Surfaces[s_FrontSurface]);
    MMIO2D::BaseAddress::WriteDestination(s_ControlAperture, s_Surfaces[s_BackSurface]);

    MMIO3D::BufferAddress::WriteColor(Function::System::s_ControlAperture, s_Surfaces[s_BackSurface]);
    MMIO3D::BufferAddress::WriteAux(Function::System::s_ControlAperture, s_DepthSurface);
}

VGA::ModeSetting::SetVideoError_t SetupBuffers(VGA::ModeSetting::Buffers_t buffers)
{
    using namespace Has;
    
    uint32_t currentMemoryAddress = 0;
    CleanUpBuffers();

    if ((VGA::ModeSetting::s_CurrentDescriptor->Flags & TDfx::Shared::Function::ModeSetting::Flags::TDfx) == 0)
    {
        //TODO: there are some modes that can do double buffering.
        if ((buffers & VGA::ModeSetting::Buffers::ImageBuffers) != VGA::ModeSetting::Buffers::SingleBuffer)
        {
            return VGA::ModeSetting::SetVideoError::UnsupportedBufferCount;
        }
        return VGA::ModeSetting::SetVideoError::Success;
    }

    s_SurfaceCount = buffers & VGA::ModeSetting::Buffers::ImageBuffers;
    s_FrontSurface = 0;
    s_BackSurface = 1 % s_SurfaceCount;
    for (uint8_t i = 0; i < s_SurfaceCount; ++i)
    {
        s_Surfaces[i] = currentMemoryAddress;
        currentMemoryAddress += VGA::ModeSetting::s_CurrentDescriptor->Stride * VGA::ModeSetting::s_CurrentDescriptor->Height;
        currentMemoryAddress = alignup<uint32_t>(currentMemoryAddress, 16);
    }

    currentMemoryAddress = alignup<uint32_t>(currentMemoryAddress, 4096);

    uint32_t fifoPages = 2;//TODO: should this be configurable??
    
    uint32_t fifoStart0 = currentMemoryAddress;
    currentMemoryAddress += fifoPages << 12;

    uint32_t fifoStart1 = currentMemoryAddress;
    currentMemoryAddress += fifoPages << 12;
    
    SetupFifos(fifoStart0 >> 12, fifoStart1 >> 12, fifoPages);

    //Todo: 2d cache...

    if (s_SurfaceCount == 3)
    {
        IO::DRAMInit1::Write(s_IOBaseAddress,
            (IO::DRAMInit1::Read(s_IOBaseAddress) & ~DRAMInit1::Buffering) |
            DRAMInit1::TripleBuffering);
    }
    else
    {
        IO::DRAMInit1::Write(s_IOBaseAddress,
            (IO::DRAMInit1::Read(s_IOBaseAddress) & ~DRAMInit1::Buffering) |
            DRAMInit1::DoubleBuffering);
    }

    if ((buffers & VGA::ModeSetting::Buffers::DepthBuffer) != VGA::ModeSetting::Buffers::DepthNone)
    {
        uint8_t depthFormat = buffers & VGA::ModeSetting::Buffers::DepthBuffer;
        if (depthFormat != VGA::ModeSetting::Buffers::Depth16Bpp)
        {
            CleanUpBuffers();
            return VGA::ModeSetting::SetVideoError::DepthBufferFormatNotSupported;
        }
        currentMemoryAddress = alignup<uint32_t>(currentMemoryAddress, 16);
        s_DepthSurface = currentMemoryAddress;

        currentMemoryAddress += alignup<uint32_t>(VGA::ModeSetting::s_CurrentDescriptor->Width * 2, 16) * VGA::ModeSetting::s_CurrentDescriptor->Height;
    }

    s_TextureMemory = currentMemoryAddress;
    s_TextureMemoryEnd = s_MemorySize << 10;
    
    if (currentMemoryAddress >= (s_MemorySize << 10))
    {
        CleanUpBuffers();
        return VGA::ModeSetting::SetVideoError::InsufficientVideoMemory;
    }
    ApplyBufferSettings();

    return VGA::ModeSetting::SetVideoError::Success;
}

void CleanUpBuffers()
{
    ShutdownFifos();

    s_SurfaceCount = 0;
    s_FrontSurface = 0;
    s_BackSurface = 0;
    s_Surfaces[0] = s_Surfaces[1] = s_Surfaces[2] = 0;

    s_DepthSurface = 0;
    s_TextureMemory = 0;
    s_TextureMemoryEnd = 0;
}

Fifo::Command_t volatile* Allocate0(uint8_t numberOfEntries)
{
    if (s_FifoHandler0.AvailableEntries > numberOfEntries)
    {
        s_FifoHandler0.AvailableEntries -= numberOfEntries;
        Fifo::Command_t volatile * ptr = (Fifo::Command_t*)s_FifoHandler0.CurrentAddress;
        s_FifoHandler0.CurrentAddress += numberOfEntries;
        s_FifoHandler0.EntriesToBump += numberOfEntries;
        return ptr;
    }
    return s_FifoHandler0.Allocate(numberOfEntries);
}

Fifo::Command_t volatile* Allocate1(uint8_t numberOfEntries)
{
    if (s_FifoHandler1.AvailableEntries > numberOfEntries)
    {
        s_FifoHandler1.AvailableEntries -= numberOfEntries;
        Fifo::Command_t volatile * ptr = (Fifo::Command_t*)s_FifoHandler1.CurrentAddress;
        s_FifoHandler1.CurrentAddress += numberOfEntries;
        s_FifoHandler1.EntriesToBump += numberOfEntries;
        return ptr;
    }
    return s_FifoHandler0.Allocate(numberOfEntries);
}

void CommitFifoEntries0()
{
    s_FifoHandler0.Bump();
}

void CommitFifoEntries1()
{
    s_FifoHandler1.Bump();
}

void SwapScreen2D(bool waitForVSync)
{
    using namespace Hag;
    using namespace Hag::System;

    if (waitForVSync) HAG_LIKELY
    {
/*
        Fifo::Packet2::Emit<0, Fifo::Packet2::Registers<
            Fifo::Packet2::Bit::CommandExtra,
            Fifo::Packet2::Bit::Command>>(
                int(TwoD::CommandExtra::WaitForVSync), int(TwoD::Command::CommandNop));
        Fifo::Packet2::Emit<0, Fifo::Packet2::Registers<
            Fifo::Packet2::Bit::CommandExtra,
            Fifo::Packet2::Bit::Command>>(
                0, int(TwoD::Command::CommandNop));
*/
        while ((VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06) & VGA::InputStatus1::VerticalSync) == VGA::InputStatus1::VerticalSyncActive);
        while ((VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06) & VGA::InputStatus1::VerticalSync) == VGA::InputStatus1::VerticalSyncInactive);
    }
    SYS_Barrier();
/*
    uint32_t* volatile readPtr0 = nullptr;
    uint32_t* volatile readPtr1 = nullptr;
    do
    {
        readPtr0 = (uint32_t * volatile)(s_LinearFrameBuffer + Fifo::CommandReadPointerLow::Read<0>(s_ControlAperture));
        readPtr1 = (uint32_t * volatile)(s_LinearFrameBuffer + Fifo::CommandReadPointerLow::Read<1>(s_ControlAperture));
    } while ((readPtr0 != s_FifoHandler0.CurrentAddress) || (readPtr1 != s_FifoHandler1.CurrentAddress));
*/
    s_FrontSurface = (s_FrontSurface + 1) % s_SurfaceCount;
    s_BackSurface = (s_BackSurface + 1) % s_SurfaceCount;
    ApplyBufferSettings();
}

}