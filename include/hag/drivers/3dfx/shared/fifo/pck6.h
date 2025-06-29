//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

// CMDFIFO Packet Type 6
// CMDFIFO Packet Type 6 is a fixed length packet, requiring 5 32-bit words for the complete packet.
// CMDFIFO Packet Type 6 is primarily used to transfer data from system AGP memory into frame buffer
// local memory. Bits 20:5 of word 0 define the transfer size in bytes of an AGP transfer. Bits 4:3 define the
// destination memory space LFB, Planar YUV , 3D LFB, and texture port. Word 1 bits 31:0, and word 2 bits
// 27:24 define the source system AGP address of the data move. Bits 23:12 define the stride, and bits 11:0
// define the width of the source surface in AGP memory. Word 3 defines the destination frame buffer
// address, while word 4 bits 11:0 define the stride of the destination surface.

// CMDFIFO Packet Type 6
//          +----------+--------------------------------------------+------+-----+
//          | 31    26 | 25                                       5 | 4  3 | 2 0 |
//          +----------+--------------------------------------------+------+-----+
// word 0   | Reserved | Number of transfer bytes [19:0]            | Type | 110 |
//          +----------+---------------------------------------------------+-----+
// word 1   | AGP [31:0]                                                         |
//          +-------------+------------------------------------+-----------------+
//          | 31       28 | 27                              14 | 13            0 |
//          +-------------+------------------------------------+-----------------+
// word 2   | AGP [35:32] | AGP Stride [13:0]                  |  Width [13:0]   |
//          +-------------+--------------------------+---------+-----------------+
//          | 31       26 | 25                                                 0 |
//          +-------------+------------------------------------------------------+
// word 3   | Reserved    | Frame buffer offset [25:0]                           |
//          +-------------+------------------------------------------------------+
//          | 31                                  15 | 14                      0 |
//          +----------------------------------------+---------------------------+
// word 4   | Reserved                               | Destination Stride [14:0] |
//          +----------------------------------------+---------------------------+

// Type     Space
// 00       Linear FB
// 01       Planar YUV
// 10       3D LFB
// 11       Texture Port

namespace Hag::TDfx::Shared::Fifo::Packet6
{

    enum
    {
        PacketType =        0x00000006,
        Type =              0x00000018,
        LinearFB =          0x00000000,
        PlanarYUV =         0x00000008,
        LinearFB3D =        0x00000010,
        TexturePort =       0x00000018,
        TransferBytes =     0x03ffffe0,
        Width =             0x00003fff,
        AGP31_0 =           0xffffffff,
        AGPStride =         0x0fffc000,
        AGP35_32 =          0xf0000000,
        FramebufferOffset = 0x03ffffff,
        DestinationStride = 0x00007fff,
    };

}