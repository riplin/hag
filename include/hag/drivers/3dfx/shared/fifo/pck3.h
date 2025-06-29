//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

// CMDFIFO Packet Type 3
// CMDFIFO Packet Type 3 is a variable length packet, requiring a minimum of 3 32-bit words, and a
// maximum of 16 vertex data groups, where a data group is all the register writes specified in the
// parameter mask, for the complete packet. It is a requirement that bits 9:6 must be greater than 0. The
// base address for CMDFIFO Packet Type 3 is defined to be the starting address of the hardware triangle
// setup registers. The first 32-bit word of the packet defines 16 individual vertex data. Bits 31:29 of word 0
// define 0 to 7 dummy fifo entries following the packet type 3 data. The sSetupMode register is written
// with the data in bits 27:10 of word 0. Bits 9:6 define the number of vertex writes contained in the packet,
// where the total packet size becomes what is defined in the parameter mask multiplied by the number of
// vertices. During parsing and execution of a CMDFIFO Packet Type 3, a specific action takes place based
// on bits 5:3. The sSetupMode register implies that X and Y are present in words 1 and 2. When Bit 28
// when set, packed color data follows the X and Y values, otherwise independent red, green, blue, and alpha
// follow X and Y data. When Smode field is 0, then word 0 defines X, and word 1 defines Y.

// Code 000 specifies an independent triangle packet, where an implied sBeginTriCMD is written after 2
// sDrawTriCMD’s. The sequence would follow, sBeginTriCMD, sDrawTriCMD, sDrawTriCMD,
// sBeginTriCMD, until “Num Vertex” vertices has been executed.

// Code 001 specifies the beginning of a triangle strip, an implicit write to sBeginTriCMD is issued,
// followed by Num Vertex sDrawTriCMD writes. The sequence would follow, sBeginTriCMD,
// sDrawTriCMD, sDrawTriCMD, sDrawTriCMD, until “Num Vertex” vertices has been executed.

// Code 010 specifies the continuance of an existing triangle strip, an implicit write to sDrawTriCMD is
// performed after one complete vertex has been parsed.

// CMDFIFO Packet Type 3
//          +------------+----+-------+----------------+------------+------+-----+
//          | 31      29 | 28 | 27 22 | 21          10 | 9        6 | 5  3 | 2 0 |
//          +------------+----+-------+----------------+------------+------+-----+
// word 0   | Number     | PC | SMode | Parameter Mask | Num Vertex | CMD  | 011 |
//          +------------+----+-------+----------------+------------+------+-----+
// word 1   |                             Data                                   |
//          +--------------------------------------------------------------------+
// word N   |                        Optional Data N                             |
//          +--------------------------------------------------------------------+

// Code     Command
// 000      Independent Triangle
// 001      Start new triangle strip
// 010      Continue existing triangle strip
// 011      reserved
// 1xx      reserved

// Bit      Description
//          sParamMask field
// 10       Setup Red, Green, and Blue
// 11       Setup Alpha
// 12       Setup Z
// 13       Setup Wb
// 14       Setup W0
// 15       Setup S0 and T0
// 16       Setup W1
// 17       Setup S1 and T1

// Bit      Description
//          sSetupMode field
// 22       Strip mode (0=strip, 1=fan)
// 23       Enable Culling (0=disable, 1=enable)
// 24       Culling Sign (0=positive sign, 1=negative sign)
// 25       Disable ping pong sign correction during triangle strips (0=normal, 1=disable)

//          Description
//          Parameter
// word 1   X
// word 2   Y
// word 3   Red / Packed ARGB (optional)
// word 4   Green (optional)
// word 5   Blue (optional)
// word 6   Alpha (optional)
// word 7   Z (optional)
// word 8   Wbroadcast (optional)
// word 9   W0 Tmu 0 & Tmu1 W (optional)
// word 10  S0 Tmu0 & Tmu1 S (optional)
// word 11  T0 Tmu0 & Tmu1 T (optional)
// word 12  W1 Tmu 1 W (optional)
// word 13  S1 Tmu1 S (optional)
// word 14  T1 Tmu1 T (optional)

// Sequence of implied commands for each code follows:
// M = Mode register write
// B = sBeginTriCMD
// D = sDrawTriCMD
// Code 000: MBDDBDDBDDBDD...
// Code 001: MBDDDDDDDDDDD...
// Code 010: MDDDDDDDDDDDD...

namespace Hag::TDfx::Shared::Fifo::Packet3
{

    enum
    {
        PacketType =            0x00000003,
        Command =               0x00000038,
        CommandTriangle =       0x00000000,
        CommandStrip =          0x00000008,
        CommandContinueStrip =  0x00000010,
        NumVertex =             0x000003c0,
        SetupRGB =              0x00000400,
        SetupAlpha =            0x00000800,
        SetupZ =                0x00001000,
        SetupWb =               0x00002000,
        SetupW0 =               0x00004000,
        SetupS0T0 =             0x00008000,
        SetupW1 =               0x00010000,
        SetupS1T1 =             0x00020000,
        Mode =                  0x00400000,
        ModeStrip =             0x00000000,
        ModeFan =               0x00400000,
        Culling =               0x00800000,
        CullingDisable =        0x00000000,
        CullingEnable =         0x00800000,
        CullingSign =           0x01000000,
        CullingPositive =       0x00000000,
        CullingNegative =       0x01000000,
        FanSignCorrection =     0x02000000,
        SignCorrectionEnable =  0x00000000,
        SignCorrectionDisable = 0x02000000
    };

}