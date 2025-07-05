
//Copyright 2025-Present riplin

#include <hag/testing/log.h>
#include <hag/drivers/3dfx/shared/fifo/fifodefr.h>

namespace Hag::TDfx::Shared::Fifo::Deferred::TwoD
{

    Command_t s_Registers[Packet2::Index::Command + 1] = { 0 };
    uint8_t s_Count;
    uint32_t s_Mask;

    void Commit(uint8_t fifo)
    {
        LOG("Fifo::Deferred::TwoD", "Commit s_Count : %i", s_Count);
        LOG("Fifo::Deferred::TwoD", "Commit s_Mask: 0x%08X", s_Mask);

        if (s_Count == 0)
            return;

        Command_t* src = s_Registers;

        Command_t volatile* dst = (fifo == 0) ? Function::System::Allocate0(s_Count + 1) : Function::System::Allocate1(s_Count + 1);

        LOG("Fifo::Deferred::TwoD", "Commit Header: 0x%08lX", Packet2::Word0(s_Mask));
        *(dst++) = Packet2::Word0(s_Mask);

        while(s_Mask != 0)
        {
            if ((s_Mask & 1) != 0)
            {
                LOG("Fifo::Deferred::TwoD", "Commit Reg: 0x%08lX", *src);
                *(dst++) = *src;
            }

            ++src;
            s_Mask >>= 1;
        }
        s_Count = 0;

        if (fifo == 0)
            Function::System::CommitFifoEntries0();
        else
            Function::System::CommitFifoEntries1();
    }

}