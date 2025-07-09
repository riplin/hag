//Copyright 2025-Present riplin

#pragma once

#include <initializer_list>
#include <hag/testing/log.h>
#include <hag/drivers/3dfx/shared/funcs/system.h>
#include <hag/drivers/3dfx/shared/fifo/fifobase.h>
#include <hag/drivers/3dfx/shared/regs/2D/baseaddr.h>
#include <hag/drivers/3dfx/shared/regs/2D/breserr.h>
#include <hag/drivers/3dfx/shared/regs/2D/clip.h>
#include <hag/drivers/3dfx/shared/regs/2D/cmd.h>
#include <hag/drivers/3dfx/shared/regs/2D/cmdextra.h>
#include <hag/drivers/3dfx/shared/regs/2D/colkey.h>
#include <hag/drivers/3dfx/shared/regs/2D/color.h>
#include <hag/drivers/3dfx/shared/regs/2D/dstfmt.h>
#include <hag/drivers/3dfx/shared/regs/2D/lncharea.h>
#include <hag/drivers/3dfx/shared/regs/2D/lnstl.h>
#include <hag/drivers/3dfx/shared/regs/2D/lnstp.h>
#include <hag/drivers/3dfx/shared/regs/2D/pattern.h>
#include <hag/drivers/3dfx/shared/regs/2D/rop.h>
#include <hag/drivers/3dfx/shared/regs/2D/size.h>
#include <hag/drivers/3dfx/shared/regs/2D/srcfmt.h>
#include <hag/drivers/3dfx/shared/regs/2D/xy.h>

// CMDFIFO Packet Type 2
// CMDFIFO Packet Type 2 is a variable length packet, requiring a minimum of 2 32-bit words, and a
// maximum of 30 32-bit words for the complete packet. The base address for CMDFIFO Packet Type 2 is
// defined to be offset 8 of the hardware 2D registers(clip0min). The first 32-bit word of the packet defines
// individual write enables for up to 29 data words to follow. From LSB to MSB of the mask, a “1” enables
// the write and a “0” disables the write. The sequence of up to 29 32-bit data words following the mask
// modify addresses equal to the implied base address plus N where mask[N] equals “1” as N ranges from 0
// to 28. The total number of 32-bit data words following the mask is equal to the number of “1”s in the
// mask. The register mask must not be 0.

// CMDFIFO Packet Type 2
//          +--------------------------------------------------------------+-----+
//          | 31                                                         3 | 2 0 |
//          +--------------------------------------------------------------+-----+
// word 0   | 2D Register Mask                                             | 010 |
//          +--------------------------------------------------------------------+
// word 1   |                             Data                                   |
//          +--------------------------------------------------------------------+
// word N   |                        Optional Data N                             |
//          +--------------------------------------------------------------------+


namespace Hag::TDfx::Shared::Fifo::Packet2
{

    enum
    {
        PacketType =        0x00000002,
        Mask =              0xfffffffc
    };

    namespace Shift
    {
        enum
        {
            PacketType = 0x00,
            Mask = 0x03
        };
    }

    namespace Index
    {
        enum
        {
            Clip0Min = (int(TwoD::Register::Clip0Min) - int(TwoD::Register::Clip0Min)) >> 2,
            Clip0Max = (int(TwoD::Register::Clip0Max) - int(TwoD::Register::Clip0Min)) >> 2,
            DestinationBaseAddress = (int(TwoD::Register::DestinationBaseAddress) - int(TwoD::Register::Clip0Min)) >> 2,
            DestinationFormat = (int(TwoD::Register::DestinationFormat) - int(TwoD::Register::Clip0Min)) >> 2,
            SourceColorKeyMin = (int(TwoD::Register::SourceColorKeyMin) - int(TwoD::Register::Clip0Min)) >> 2,
            SourceColorKeyMax = (int(TwoD::Register::SourceColorKeyMax) - int(TwoD::Register::Clip0Min)) >> 2,
            DestinationColorKeyMin = (int(TwoD::Register::DestinationColorKeyMin) - int(TwoD::Register::Clip0Min)) >> 2,
            DestinationColorKeyMax = (int(TwoD::Register::DestinationColorKeyMax) - int(TwoD::Register::Clip0Min)) >> 2,
            BresenhamError0 = (int(TwoD::Register::BresenhamError0) - int(TwoD::Register::Clip0Min)) >> 2,
            BresenhamError1 = (int(TwoD::Register::BresenhamError1) - int(TwoD::Register::Clip0Min)) >> 2,
            RasterOperation = (int(TwoD::Register::RasterOperation) - int(TwoD::Register::Clip0Min)) >> 2,
            SourceBaseAddress = (int(TwoD::Register::SourceBaseAddress) - int(TwoD::Register::Clip0Min)) >> 2,
            CommandExtra = (int(TwoD::Register::CommandExtra) - int(TwoD::Register::Clip0Min)) >> 2,
            LineStipple = (int(TwoD::Register::LineStipple) - int(TwoD::Register::Clip0Min)) >> 2,
            LineStyle = (int(TwoD::Register::LineStyle) - int(TwoD::Register::Clip0Min)) >> 2,
            Pattern0Alias = (int(TwoD::Register::Pattern0Alias) - int(TwoD::Register::Clip0Min)) >> 2,
            Pattern1Alias = (int(TwoD::Register::Pattern1Alias) - int(TwoD::Register::Clip0Min)) >> 2,
            Clip1Min = (int(TwoD::Register::Clip1Min) - int(TwoD::Register::Clip0Min)) >> 2,
            Clip1Max = (int(TwoD::Register::Clip1Max) - int(TwoD::Register::Clip0Min)) >> 2,
            SourceFormat = (int(TwoD::Register::SourceFormat) - int(TwoD::Register::Clip0Min)) >> 2,
            SourceSize = (int(TwoD::Register::SourceSize) - int(TwoD::Register::Clip0Min)) >> 2,
            SourceXY = (int(TwoD::Register::SourceXY) - int(TwoD::Register::Clip0Min)) >> 2,
            BackgroundColor = (int(TwoD::Register::BackgroundColor) - int(TwoD::Register::Clip0Min)) >> 2,
            ForegroundColor = (int(TwoD::Register::ForegroundColor) - int(TwoD::Register::Clip0Min)) >> 2,
            DestinationSize = (int(TwoD::Register::DestinationSize) - int(TwoD::Register::Clip0Min)) >> 2,
            DestinationXY = (int(TwoD::Register::DestinationXY) - int(TwoD::Register::Clip0Min)) >> 2,
            Command = (int(TwoD::Register::Command) - int(TwoD::Register::Clip0Min)) >> 2,
        };
    }

    namespace Bit
    {
        enum
        {
            Clip0Min =                  1 << Index::Clip0Min,
            Clip0Max =                  1 << Index::Clip0Max,
            DestinationBaseAddress =    1 << Index::DestinationBaseAddress,
            DestinationFormat =         1 << Index::DestinationFormat,
            SourceColorKeyMin =         1 << Index::SourceColorKeyMin,
            SourceColorKeyMax =         1 << Index::SourceColorKeyMax,
            DestinationColorKeyMin =    1 << Index::DestinationColorKeyMin,
            DestinationColorKeyMax =    1 << Index::DestinationColorKeyMax,
            BresenhamError0 =           1 << Index::BresenhamError0,
            BresenhamError1 =           1 << Index::BresenhamError1,
            RasterOperation =           1 << Index::RasterOperation,
            SourceBaseAddress =         1 << Index::SourceBaseAddress,
            CommandExtra =              1 << Index::CommandExtra,
            LineStipple =               1 << Index::LineStipple,
            LineStyle =                 1 << Index::LineStyle,
            Pattern0Alias =             1 << Index::Pattern0Alias,
            Pattern1Alias =             1 << Index::Pattern1Alias,
            Clip1Min =                  1 << Index::Clip1Min,
            Clip1Max =                  1 << Index::Clip1Max,
            SourceFormat =              1 << Index::SourceFormat,
            SourceSize =                1 << Index::SourceSize,
            SourceXY =                  1 << Index::SourceXY,
            BackgroundColor =           1 << Index::BackgroundColor,
            ForegroundColor =           1 << Index::ForegroundColor,
            DestinationSize =           1 << Index::DestinationSize,
            DestinationXY =             1 << Index::DestinationXY,
            Command =                   1 << Index::Command,
        };
    }

    inline Command_t Word0(uint32_t mask) { return PacketType | (mask << Shift::Mask); }

    template <auto... Regs>
    struct Registers
    {
        static consteval std::size_t length() { return sizeof...(Regs); }

        static consteval Command_t bits()
        {
            Command_t ret = 0;
            for (auto reg : {Regs...})
            {
                ret |= reg;
            }
            return ret;
        }
    };
    
    template <int F, typename Regs, typename... Vals>
    forceinline void Emit(Vals... vals)
    {
        static_assert(Regs::length() == sizeof...(Vals), "Number of registers must match number of arguments");
        static_assert(sizeof...(Vals) > 0, "Must write at least one register");

        Command_t volatile* ptr = nullptr;

        if constexpr (F == 0)
            ptr = Function::System::Allocate0(1 + Regs::length());
        else
            ptr = Function::System::Allocate1(1 + Regs::length());

            
        int index = 0;
        ptr[index] = (Regs::bits() << Shift::Mask) | PacketType;
        LOG("Fifo", "Packet2: header: 0x%08lX", ptr[index]);
        ++index;
        
        for (auto val : {vals...})
        {
            LOG("Fifo", "Packet2: register: 0x%08lX", val);

            ptr[index] = val;
            ++index;
        }

        if constexpr (F == 0)
            Function::System::CommitFifoEntries0();
        else
            Function::System::CommitFifoEntries1();
    }

}

