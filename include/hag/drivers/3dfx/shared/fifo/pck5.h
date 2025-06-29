//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

// CMDFIFO Packet Type 5
// CMDFIFO Packet Type 5 is a variable length packet, requiring a minimum of 3 32-bit words, and a
// maximum of 2^19 32-bit words for the complete packet Bits 31:30 define LFB type, one of linear frame
// buffer, planar YUV space, 3D LFB, or texture download port Bits 29:26 in word 0 define the byte enables
// for word 2, and are active low true. Bits 25:22 in word 0 define the byte enables for word N. Data must
// be in the correct data lane, and the base address must be 32-bit aligned. CMDFIFO Packet Type 5 is used
// to transfer large consecutive quantities of data from the CPU to the accelerator’s frame buffer with proper
// order with the command stream One note, transfer to tile space is limited if tile-stride does not match PCI
// stride. Tile space rows are not continuous, thus each tile row must be separated into separate packets.

// CMDFIFO Packet Type 5
//          +---------+----------------+----------------+------------------+-----+
//          | 31   30 | 29          26 | 25          22 | 21             3 | 2 0 |
//          +---------+----------------+----------------+------------------+-----+
// word 0   | Space   | Byte Enable W2 | Byte Enable WN | Num Words        | 101 |
//          +---------+----------------+----------------+------------------+-----+
// word 1   | Reseved | Base Address [24:0]                                      |
//          +--------------------------------------------------------------------+
// word 2   |                             Data                                   |
//          +--------------------------------------------------------------------+
// word N   |                        Optional Data N                             |
//          +--------------------------------------------------------------------+

// Code     Space
// 00       Linear FB
// 01       Planar YUV
// 10       3D LFB
// 11       Texture Port

namespace Hag::TDfx::Shared::Fifo::Packet5
{

    enum
    {
        PacketType =        0x00000005,
        NumWords =          0x003ffffc,
        ByteEnableWN =      0x03c00000,
        ByteEnableW2 =      0x3c000000,
        Space =             0xc0000000,
        LinearFB =          0x00000000,
        PlanarYUV =         0x40000000,
        LinearFB3D =        0x80000000,
        TexturePort =       0xc0000000
    };

}