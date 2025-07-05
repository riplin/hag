//Copyright 2025-Present riplin

#pragma once

#include <assert.h>
#include <string.h>
#include <hag/drivers/3dfx/shared/fifo/pck4.h>
#include <hag/drivers/3dfx/shared/funcs/system.h>
#include <hag/drivers/3dfx/shared/regs/2D/pattern.h>
#include <hag/drivers/3dfx/shared/regs/2D/lncharea.h>

namespace Hag::TDfx::Shared::Fifo::Function
{

    template <int F>
    void WriteLaunchArea(const Shared::TwoD::LaunchCommand_t* sourceCommands, uint16_t commandCount)
    {
        uint8_t maxEntries = numbits(Packet4::GeneralRegisterMask);

        uint8_t launchOffset = 0;
        while(commandCount != 0)
        {
            uint8_t entries = min<uint8_t>(commandCount, maxEntries);
            uint16_t mask = Packet4::GeneralRegisterMask >> ((maxEntries - entries) + Packet4::Shift::GeneralRegisterMask);

            Command_t volatile* ptr = nullptr;
            if constexpr (F == 0)
                ptr = Shared::Function::System::Allocate0(entries + 1);
            else
                ptr = Shared::Function::System::Allocate1(entries + 1);

            *(ptr++) = Packet4::Word0_2D(launchOffset + (Shared::TwoD::Register::LaunchArea >> 2), mask, 0);

            memcpy((void*)ptr, sourceCommands, entries * sizeof(Shared::TwoD::LaunchCommand_t));
            sourceCommands += entries;

            if constexpr (F == 0)
                Shared::Function::System::CommitFifoEntries0();
            else
                Shared::Function::System::CommitFifoEntries1();

            launchOffset = (launchOffset == 0) ? 14 : 0;
            commandCount -= entries;
        }
    }

    template <int F>
    void WritePattern(const Shared::TwoD::Pattern_t* pattern, uint8_t patternCount)
    {
        assert(patternCount <= 64);
        uint8_t maxEntries = numbits(Packet4::GeneralRegisterMask);
        uint8_t patternOffset = 0;
        while(patternCount != 0)
        {
            uint8_t entries = min<uint8_t>(patternCount, maxEntries);
            uint16_t mask = Packet4::GeneralRegisterMask >> ((maxEntries - entries) + Packet4::Shift::GeneralRegisterMask);

            Command_t volatile* ptr = nullptr;
            if constexpr (F == 0)
                ptr = Shared::Function::System::Allocate0(entries + 1);
            else
                ptr = Shared::Function::System::Allocate1(entries + 1);

            *(ptr++) = Packet4::Word0_2D(patternOffset + (Shared::TwoD::Register::Pattern >> 2), mask, 0);

            memcpy((void*)ptr, pattern, entries * sizeof(Shared::TwoD::LaunchCommand_t));
            pattern += entries;

            if constexpr (F == 0)
                Shared::Function::System::CommitFifoEntries0();
            else
                Shared::Function::System::CommitFifoEntries1();

            patternOffset += entries;
            patternCount -= entries;
        }
    }

}