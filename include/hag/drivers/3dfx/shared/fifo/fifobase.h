//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared::Fifo
{

typedef uint32_t Command_t;
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
