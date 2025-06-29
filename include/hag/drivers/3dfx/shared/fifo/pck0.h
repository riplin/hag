//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

// CMDFIFO Packet Type 0
// CMDFIFO Packet Type 0 is a variable length packet, requiring a minimum single 32-bit word, to a
// maximum of 2 32-bit words. CMDFIFO Packet Type 0 is used to jump to the beginning of the fifo when
// the end of the fifo is reached. CMDFIFO Packet Type 0 also supports jumping to a secondary command
// stream just like a jump subroutine call (jsr instruction), with a CMDFIFO Packet that instructs a return as
// well. NOP , JSR, RET, and JMP LOCAL FRAME BUFFER functions only require a single 32-bit word
// CMDFIFO packet, while the JMP AGP function requires a two 32-bit word CMDFIFO packet. Bits 31:29
// are reserved and must be written with 0.

// CMDFIFO Packet Type 0
//          +----------+--------------------------------------------+------+-----+
//          | 31    29 | 28                                       6 | 5  3 | 2 0 |
//          +----------+--------------------------------------------+------+-----+
// word 0   | Reserved | Address [24:2]                             | Func | 000 |
//          +----------+----------------------------+----------------------------+
//          | 31                                 11 | 10                       0 |
//          +---------------------------------------+----------------------------+
// word 1   | Reserved                              | Address [35:25]            |
//          +--------------------------------------------------------------------+

namespace Hag::TDfx::Shared::Fifo::Packet0
{

    enum
    {
        PacketType =        0x00000000,
        Opcode =            0x00000038,
        Nop =               0x00000000,
        Jsr =               0x00000008,
        Ret =               0x00000010,
        JmpLocal =          0x00000018, // Jump local framebuffer
        JmpAgp =            0x00000020, // Jump AGP
        Address24_2 =       0x1fffffc0, // Word 0
        Address35_25 =      0x000007ff  // Word 1
    };

}