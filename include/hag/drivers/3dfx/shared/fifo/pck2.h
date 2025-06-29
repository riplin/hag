//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

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

}