//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared::Fifo
{

// Banshee supports two full CMDFIFO streams and each individually can be located in frame buffer
// memory or AGP space. Each CMDFIFO has its own base address register set, that defines the starting
// address, memory space, and size of the CMDFIFO. The CMDFIFO registers contain a write only bump
// register that increments the write pointer by the amount written to the cmdBump register. Each
// CMDFIFO contains a read pointer, write pointer, and freespace count of the fifo itself, so the CPU can
// monitor the progress and fullness of the CMDFIFO. Ordering between the two CMDFIFO’s is first come,
// first served.

namespace Register
{
    enum
    {
        Base = 0x0080000
    };
}

}

// Register Name        Address     Bits    R/W     Description
//      AGP
// agpReqSize           0x000(0)    19:0    R/W     Size of AGP request
// agpHostAddressLow    0x004(4)    31:0    R/W     Host address bits 31:0
// agpHostAddressHigh   0x008(8)    31:0    R/W     Host address bits 35:32
// agpGraphicsAddress   0x00C(12)   24:0    R/W     Graphics address bits 24:0
// agpGraphicsStride    0x010(16)   16:0    R/W     Graphics stride
// agpMoveCMD           0x014(20)    n/a      W     Begin AGP transaction

//   CMDFIFO 0
// cmdBaseAddr0         0x020(32)   23:0    R/W     Base Address of CMDFIFO 0
// cmdBaseSize0         0x024(36)   10:0    R/W     CMDFIFO0 size
// cmdBump0             0x028(40)   15:0      W     Bump CMDFIFO 0 N words
// cmdRdPtrL0           0x02c(44)   31:0    R/W     CMDFIFO 0 read pointer lower 32 bits
// cmdRdPtrH0           0x030(48)    3:0    R/W     CMDFIFO 0 read pointer upper 4 bits
// cmdAMin0             0x034(52)   24:0    R/W     CMDFIFO 0 address min pointer
//                      0x038                       Reserved
// cmdAMax0             0x03c(60)   24:0    R/W     CMDFIFO 0 address max pointer
//                      0x040(64)                   Reserved
// cmdFifoDepth0        0x044(68)   19:0    R/W     CMDFIFO 0 depth value
// cmdHoleCnt0          0x048(72)   15:0    R/W     CMDFIFO 0 outstanding CPU writes

//   CMDFIFO 1
// cmdBaseAddr1         0x050(80)   23:0    R/W     Base Address of CMDFIFO 1
// cmdBaseSize1         0x054(84)   10:0    R/W     CMDFIFO1 size
// cmdBump1             0x058(88)   15:0      W     Bump CMDFIFO 1 N words
// cmdRdPtrL1           0x05c(92)   31:0    R/W     CMDFIFO 1 read pointer lower 32 bits
// cmdRdPtrH1           0x060(96)    3:0    R/W     CMDFIFO 1 read pointer upper 4 bits
// cmdAMin1             0x064(100)  24:0    R/W     CMDFIFO 1 address min pointer
//                      0x068(104)                  Reserved
// cmdAMax1             0x06c(108)  24:0    R/W     CMDFIFO 1 address max pointer
//                      0x070(112)                  Reserved
// cmdFifoDepth1        0x074(116)  19:0    R/W     CMDFIFO 1 freespace
// cmdHoleCnt1          0x078(120)  15:0    R/W     CMDFIFO 1 outstanding CPU writes

// cmdFifoThresh        0x080(96)   4:0     R/W     CMDFIFO fetch threshold
// cmdHoleInt           0x084(100) 22:0     R/W     Cmd hole timeout value

//     Misc
// yuvBaseAddress       0x100(256) 25:0     R/W     YUV planar base address
// yuvStride            0x104(260) 12:0     R/W     Y, U and V planes stride value
// crc1                 0x120(288) 31:0     R/W     CRC in control path (for testing purposes only)
// crc2                 0x130(304) 31:0     R       Video CRC output.

// XXXXXXXOXXOXXXOOXO
// ^ ^   ^         ^
// | |   |         |
// | |   |         +--- amax
// | |   |
// | |   +------------- amin
// | |
// | +----------------- rdptr
// |
// +------------------- base addr

// X = written location     O = unwritten location

// The command registers define the location, size, and fifo management method of the command fifo. The
// command fifo starts at the address defined in the cmdBaseAddr[01] register and occupies N 4k byte
// pages defined in the cmdBaseSize register. The command fifo can be located either in AGP or frame
// buffer memory which is defined in the cmdBaseSize register. CmdRdPtr points to the last executed
// entry in the command fifo. Amin is a pointer that walks through the fifo until it reaches an unwritten
// location. The rdptr can not access any entry beyond the amin pointer. The amax pointer is set to the
// furthest address location of a given write. The hole counter is basically the number of unwritten locations
// between the amax register and the amin register. When the hole counter is zero, the amin register is set to
// the value of the amax register, thus allowing the read pointer to advance to the new amin register value.
// The depth of the fifo is calculated by the difference between amax and rdptr.

// Or, put another way (from the perspective of a driver writer):

// When hole counting is enabled (hardware manages command fifo depth), the memory controller takes
// special action whenever a write occurs between the command fifo base and the base + size. As writes
// occur in this region, five variables are fiddled: readPtr, depth, aMin, aMax, and holeCount. As ordered
// writes happen, both aMin and aMax increment, as does depth and readPtr. In this state, the difference
// between aMin/aMax and the readPtr is the depth. When the depth is nonzero, the readPtr advances as
// commands are read from the buffer. When/if an out-of-order write occurs, aMin stops incrementing, but
// aMax continues to increment as addresses written go up. The readPtr will not pass aMin, so the depth
// begins to decrement. Once the readPtr has caught up with aMin, the depth sits at zero. If aMax ever has
// to skip (due to an out-of-order write), the hole count is incremented. As out-of-order data gets written
// between aMin and aMax, the hole count is decremented. When the holeCount goes to zero, the difference
// between aMin and aMax is added to the depth, and aMin is set to be the same as aMax. This causes
// command processing to resume.