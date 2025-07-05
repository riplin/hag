//Copyright 2025-Present riplin

#pragma once

#include <stdio.h>
#include <hag/types.h>
#include <assert.h>

#include <hag/testing/log.h>

#include <hag/drivers/3dfx/shared/fifo/cmdbaddr.h>
#include <hag/drivers/3dfx/shared/fifo/cmdbsize.h>
#include <hag/drivers/3dfx/shared/fifo/cmdamin.h>
#include <hag/drivers/3dfx/shared/fifo/cmdamax.h>
#include <hag/drivers/3dfx/shared/fifo/cmdbump.h>
#include <hag/drivers/3dfx/shared/fifo/cmdfdpth.h>
#include <hag/drivers/3dfx/shared/fifo/cmdfthrs.h>
#include <hag/drivers/3dfx/shared/fifo/cmdhcnt.h>
#include <hag/drivers/3dfx/shared/fifo/cmdrdpth.h>
#include <hag/drivers/3dfx/shared/fifo/cmdrdptl.h>
#include <hag/drivers/3dfx/shared/fifo/pck0.h>

namespace Hag::TDfx::Shared::Fifo
{

// Banshee supports two full CMDFIFO streams and each individually can be located in frame buffer
// memory or AGP space. Each CMDFIFO has its own base address register set, that defines the starting
// address, memory space, and size of the CMDFIFO. The CMDFIFO registers contain a write only bump
// register that increments the write pointer by the amount written to the cmdBump register. Each
// CMDFIFO contains a read pointer, write pointer, and freespace count of the fifo itself, so the CPU can
// monitor the progress and fullness of the CMDFIFO. Ordering between the two CMDFIFO’s is first come,
// first serve.

// XXXXXXXOXXOXXXOOXO
// ^ ^   ^         ^
// | |   |         |
// | |   |         +--- amax
// | |   |
// | |   +------------- amin
// | |
// | +----------------- rdptr
// |
// +------------------- base addr

// X = written location     O = unwritten location

// The command registers define the location, size, and fifo management method of the command fifo. The
// command fifo starts at the address defined in the cmdBaseAddr[01] register and occupies N 4k byte
// pages defined in the cmdBaseSize register. The command fifo can be located either in AGP or frame
// buffer memory which is defined in the cmdBaseSize register. CmdRdPtr points to the last executed
// entry in the command fifo. Amin is a pointer that walks through the fifo until it reaches an unwritten
// location. The rdptr can not access any entry beyond the amin pointer. The amax pointer is set to the
// furthest address location of a given write. The hole counter is basically the number of unwritten locations
// between the amax register and the amin register. When the hole counter is zero, the amin register is set to
// the value of the amax register, thus allowing the read pointer to advance to the new amin register value.
// The depth of the fifo is calculated by the difference between amax and rdptr.

// Or, put another way (from the perspective of a driver writer):

// When hole counting is enabled (hardware manages command fifo depth), the memory controller takes
// special action whenever a write occurs between the command fifo base and the base + size. As writes
// occur in this region, five variables are fiddled: readPtr, depth, aMin, aMax, and holeCount. As ordered
// writes happen, both aMin and aMax increment, as does depth and readPtr. In this state, the difference
// between aMin/aMax and the readPtr is the depth. When the depth is nonzero, the readPtr advances as
// commands are read from the buffer. When/if an out-of-order write occurs, aMin stops incrementing, but
// aMax continues to increment as addresses written go up. The readPtr will not pass aMin, so the depth
// begins to decrement. Once the readPtr has caught up with aMin, the depth sits at zero. If aMax ever has
// to skip (due to an out-of-order write), the hole count is incremented. As out-of-order data gets written
// between aMin and aMax, the hole count is decremented. When the holeCount goes to zero, the difference
// between aMin and aMax is added to the depth, and aMin is set to be the same as aMax. This causes
// command processing to resume.

template <int F>
struct Handler
{
    Handler()
        : BaseOffset(0)
        , EndOffset(0)
        , EntriesToBump(0)
        , BaseAddress(nullptr)
        , EndAddress(nullptr)
        , ControlAperture(nullptr)
        , FramebufferAperture(nullptr)
    {
    }

    void Setup(uint8_t* controlAperture, uint8_t* framebufferAperture, uint32_t basePage4K, uint8_t pageCountMinusOne);
    void Shutdown();

    Command_t volatile* Allocate(uint32_t entries);
    void Bump();

    void EmitNop();
    void EmitJump(uint32_t address);
    void EmitCall(uint32_t address);
    void EmitReturn();

    uint32_t BaseOffset; // Base offset in frame buffer space
    uint32_t EndOffset; // End offset in frame buffer space
    uint32_t EntriesToBump; // Number of enqueued writes
    uint32_t AvailableEntries; // Number of open entries
    uint32_t* BaseAddress; // Base address in host memory space
    uint32_t* CurrentAddress; // Current write address in host memory space
    uint32_t* EndAddress; // End address in host memory space
    uint8_t* ControlAperture; // Address to control aperture
    uint8_t* FramebufferAperture; // Address to frame buffer aperture
};

template <int F>
void Handler<F>::Setup(uint8_t* controlAperture, uint8_t* framebufferAperture, uint32_t basePage4K, uint8_t pageCountMinusOne)
{
    BaseOffset = basePage4K << 12;
    LOG("Fifo::Handler", "BaseOffset          %i : %lu", F, BaseOffset);

    EndOffset = (pageCountMinusOne + basePage4K + 1) << 12;
    LOG("Fifo::Handler", "EndOffset           %i : %lu", F, EndOffset);

    BaseAddress = (uint32_t*)(framebufferAperture + BaseOffset);
    LOG("Fifo::Handler", "BaseAddress         %i : 0x%08lX", F, (uint32_t)BaseAddress);

    CurrentAddress = BaseAddress;
    LOG("Fifo::Handler", "CurrentAddress      %i : 0x%08lX", F, (uint32_t)CurrentAddress);

    EndAddress = (uint32_t*)(framebufferAperture + EndOffset);
    LOG("Fifo::Handler", "EndAddress          %i : 0x%08lX", F, (uint32_t)EndAddress);

    EntriesToBump = 0;
    AvailableEntries = (EndAddress - BaseAddress) - 1;
    LOG("Fifo::Handler", "AvailableEntries    %i : %lu", F, AvailableEntries);

    ControlAperture = controlAperture;
    LOG("Fifo::Handler", "ControlAperture     %i : 0x%08lX", F, (uint32_t)ControlAperture);

    FramebufferAperture = framebufferAperture;
    LOG("Fifo::Handler", "FramebufferAperture %i : 0x%08lX", F, (uint32_t)FramebufferAperture);

    CommandBaseAddress::Write<F>(ControlAperture, basePage4K);

    CommandReadPointerLow::Write<F>(ControlAperture, BaseOffset);
    CommandReadPointerHigh::Write<F>(ControlAperture, 0);

    CommandAMin::Write<F>(ControlAperture, 0);
    CommandAMax::Write<F>(ControlAperture, 0);

    CommandHoleCount::Write<F>(ControlAperture, 0);

    CommandFifoDepth::Write<F>(ControlAperture, 0);

    CommandBump::Write<F>(ControlAperture, 0);

    LOG("Fifo::Handler", "Pre-enable %i", F);
    CommandBaseSize::Write<F>(ControlAperture, pageCountMinusOne |
        CommandBaseSize::Enable |
        CommandBaseSize::Framebuffer |
        CommandBaseSize::HoleEnable);
    LOG("Fifo::Handler", "Post-enable %i", F);
}

template <int F>
void Handler<F>::Shutdown()
{
    CommandBaseSize::Write<F>(ControlAperture, CommandBaseSize::Disable);
    BaseOffset = 0;
    EndOffset = 0;
    EntriesToBump = 0;
    AvailableEntries = 0;
    BaseAddress = nullptr;
    CurrentAddress = nullptr;
    EndAddress = nullptr;
    ControlAperture = nullptr;
    FramebufferAperture = nullptr;
}

template <int F>
Command_t volatile* Handler<F>::Allocate(uint32_t entries)
{
    LOG("Fifo::Handler", "Allocate %i : %i", F, entries);
    uint32_t* volatile readPtr = nullptr;

    assert(entries < uint32_t((EndAddress - BaseAddress) - 1)); // Buffer too small!

    if ((CurrentAddress + entries) >= EndAddress)//Equal is also not enough, since we need to jump back.
    {
        LOG("Fifo::Handler", "Wrap %i", F);
        //We don't have space, so we have to wrap around to the start of the buffer.
        //We have to make sure that the read pointer isn't ahead of us, so wait for it to wrap.

        do
        {
            readPtr = (uint32_t* volatile)(FramebufferAperture + CommandReadPointerLow::Read<F>(ControlAperture));
        } while ((readPtr > CurrentAddress) || (readPtr == BaseAddress));//Never let CurrentAddress become readPtr

        //Wrap to the start of the buffer.
        *((uint32_t volatile*)CurrentAddress) = Packet0::Word0(Packet0::JmpLocal, BaseOffset);
        LOG("Fifo::Handler", "Jump command: 0x%08lX", *CurrentAddress);
        EntriesToBump += 1;
        Bump();

        LOG("Fifo::Handler", "Wrapped %i", F);

        //Reset to start of buffer
        CurrentAddress = BaseAddress;
        AvailableEntries = 0; //Recalculate
    }

    while (AvailableEntries <= entries)
    {
        readPtr = (uint32_t* volatile)(FramebufferAperture + CommandReadPointerLow::Read<F>(ControlAperture));
        LOG("Fifo::Handler", "while: CurrentAddress: 0x%08lX, readPtr: 0x%08lX, EndAddress: %08lX", CurrentAddress, readPtr, EndAddress);
        
        if (CurrentAddress > readPtr)
            AvailableEntries = (EndAddress - CurrentAddress) - 1; // We are ahead of readPtr, save space for jmp
        else
            AvailableEntries = (readPtr - CurrentAddress) - 1; // We are following readPtr, don't catch up.
    }

    EntriesToBump += entries;
    AvailableEntries -= entries;
    uint32_t* writePtr = CurrentAddress;
    CurrentAddress += entries;

    return writePtr;
}

template <int F>
void Handler<F>::Bump()
{
    LOG("Fifo::Handler", "Bump %i", EntriesToBump);
    if (EntriesToBump != 0)    
        CommandBump::Write<F>(ControlAperture, EntriesToBump);
    EntriesToBump = 0;
}

template <int F>
void Handler<F>::EmitNop()
{
    LOG("Fifo::Handler", "Nop");
    Command_t volatile* ptr = Allocate(1);
    *ptr = Packet0::Word0(Packet0::Nop, 0);
    Bump();
}

template <int F>
void Handler<F>::EmitJump(uint32_t address)
{
    LOG("Fifo::Handler", "Jump 0x%08lX", address);
    Command_t volatile* ptr = Allocate(1);
    *ptr = Packet0::Word0(Packet0::JmpLocal, address);
    Bump();
}

template <int F>
void Handler<F>::EmitCall(uint32_t address)
{
    LOG("Fifo::Handler", "Call 0x%08lX", address);
    Command_t volatile* ptr = Allocate(1);
    *ptr = Packet0::Word0(Packet0::Jsr, address);
    Bump();
}

template <int F>
void Handler<F>::EmitReturn()
{
    LOG("Fifo::Handler", "Return");
    Command_t volatile* ptr = Allocate(1);
    *ptr = Packet0::Word0(Packet0::Ret, 0);
    Bump();
}

}