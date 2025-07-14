//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>
#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

#define HAG_3DFX_SYSTEM                                     \
    using namespace Hag::TDfx::Shared::Function::System;

namespace Hag::TDfx::Shared::Function::System
{

    extern uint32_t GetMemorySize(); // Returns video memory size in bytes

    //Fifo handling

    // Allocate entries in fifo 0
    extern Fifo::Command_t volatile* Allocate0(uint8_t numberOfEntries);

    // Allocate entries in fifo 1
    extern Fifo::Command_t volatile* Allocate1(uint8_t numberOfEntries);

    // Notify fifo 0 that there are entries to be processed
    extern void CommitFifoEntries0();

    // Notify fifo 1 that there are entries to be processed
    extern void CommitFifoEntries1();

}