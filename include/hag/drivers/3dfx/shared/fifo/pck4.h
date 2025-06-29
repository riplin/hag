//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

// CMDFIFO Packet Type 4
// CMDFIFO Packet Type 4 is a variable length packet, requiring a minimum of 2 32-bit words, and a
// maximum of 22 32-bit words for the complete packet. The first 3 bits 31:29 of word 0 define the number
// of pad words that follow the packet type 4 data. The next 14 bits of the header 28:15 define the register
// write mask, followed by the register base field, described later in this section. From LSB to MSB of the
// mask, a “1” enables the write and a “0” disables the write. The sequence of up to 22 32-bit data words
// following the mask modify addresses equal to the implied base address plus N where mask[N] equals “1”
// as N ranges from 0 to 16. The total number of 32-bit data words following the mask is equal to the
// number of “1”s in the mask. As a requirement, the general register mask must have a non zero value.

// CMDFIFO Packet Type 4
//          +--------+-------------------------+----------------------------+-----+
//          | 31  29 | 28                   15 | 14                       3 | 2 0 |
//          +--------+-------------------------+----------------------------+-----+
// word 0   | Number | General Register Mask   | Register Base (See below)  | 100 |
//          +----------------------------------+----------------------------+-----+
// word 1   |                              Data                                   |
//          +---------------------------------------------------------------------+
// word N   |                         Optional Data N                             |
//          +---------------------------------------------------------------------+

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


namespace Hag::TDfx::Shared::Fifo::Packet4
{

    enum
    {
        PacketType =            0x00000004,
        RegisterBase =          0x00007ff8,
        RegisterType =          0x00004000,
        Type2D =                0x00004000,
        Type3D =                0x00000000,
        Chip =                  0x00001800,
        Register =              0x000007f8,
        GeneralRegisterMask =   0x1fff8000,
        Number =                0xe0000000
    };

}