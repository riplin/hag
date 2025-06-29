//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

// CMDFIFO Packet Type 1
// CMDFIFO Packet Type 1 is a variable length packet that allows writes to either a common address, or to
// consecutive addresses, minimum number of words is 2 32-bit words, and maximum number of words is
// 65536 words. Bits 31:16 define the number of words that follow word 0 of packet type 1, and must be
// greater than 0. When bit 15 is a 1, data following word 0 in the packet is written in consecutive addresses
// starting from the register base address defined in bits 14:3. When bit 15 is a 0, data following word 0 is
// written to the base address. Packet header bits 14:3 define the base address of the packet, see section
// below. The common use of packet type 1 is host blits.

// CMDFIFO Packet Type 1
//          +---------------------------+-----+----------------------------+-----+
//          | 31                     16 |  15 | 14                       3 | 2 0 |
//          +---------------------------+-----+----------------------------+-----+
// word 0   | Number of words           | inc | Register Base (See below)  | 001 |
//          +---------------------------+-----+----------------------------+-----+
// word 1   |                             Data                                   |
//          +--------------------------------------------------------------------+
// word N   |                        Optional Data N                             |
//          +--------------------------------------------------------------------+

// Register base:
//                  Banshee
// +--------------------------------------------+
// |   14  |    13    | 12 11 | 10            3 |
// +--------------------------------------------+
// | 2D/3D | Reserved | Chip  | Register Number |
// +--------------------------------------------+
// If bit 11 is set, then the register base is a 2D register, otherwise it is a 3D register.

//                  CVG
// +--------------------------------------------+
// |  14                   11 | 10            3 |
// +--------------------------------------------+
// | Chip field               | Register Number |
// +--------------------------------------------+

namespace Hag::TDfx::Shared::Fifo::Packet1
{

    enum
    {
        PacketType =        0x00000001,
        RegisterBase =      0x00007ff8,
        RegisterType =      0x00004000,
        Type2D =            0x00004000,
        Type3D =            0x00000000,
        Chip =              0x00001800,
        Register =          0x000007f8,
        Increment =         0x00008000,
        WordCount =         0xffff0000,
    };

}