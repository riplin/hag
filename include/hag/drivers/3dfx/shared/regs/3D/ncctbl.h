//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        NccTable0 = 0x324,
        NccTable1 =0x354
    };
}

// nccTable0 and nccTable1 Registers
// The nccTable0 and nccTable1 registers contain two Narrow Channel Compression (NCC) tables used to
// store lookup values for compressed textures (used in YIQ and AYIQ texture formats as specified in
// tformat of textureMode). Two tables are stored so that they can be swapped on a per-triangle basis when
// performing multi-pass rendering, thus avoiding a new download of the table. Use of either nccTable0 or
// nccTable1 is selected by the Narrow Channel Compressed (NCC) Table Select bit of textureMode.
// nccTable0 and nccTable1 are stored in the format of the table below, and are write only.

// nccTable Address    Bits        Contents
// 0                   31:0        {Y3[7:0], Y2[7:0], Y1[7:0], Y0[7:0]}
// 1                   31:0        {Y7[7:0], Y6[7:0], Y5[7:0], Y4[7:0]}
// 2                   31:0        {Yb[7:0], Ya[7:0], Y9[7:0], Y8[7:0]}
// 3                   31:0        {Yf[7:0], Ye[7:0], Yd[7:0], Yc[7:0]}
// 4                   26:0        {I0_r[8:0], I0_g[8:0], I0_b[8:0]}
// 5                   26:0        {I1_r[8:0], I1_g[8:0], I1_b[8:0]}
// 6                   26:0        {I2_r[8:0], I2_g[8:0], I2_b[8:0]}
// 7                   26:0        {I3_r[8:0], I3_g[8:0], I3_b[8:0]}
// 8                   26:0        {Q0_r[8:0], Q0_g[8:0], Q0_b[8:0]}
// 9                   26:0        {Q1_r[8:0], Q1_g[8:0], Q1_b[8:0]}
// 10                  26:0        {Q2_r[8:0], Q2_g[8:0], Q2_b[8:0]}
// 11                  26:0        {Q3_r[8:0], Q3_g[8:0], Q3_b[8:0]}

// The following figure illustrates how compressed textures are decompressed using the NCC tables:

//  nccTable register           From Memory Data Alignment
//  Select 
//      |            |                    |                    |
//      |            / 4 Y                / 2 I                / 2 Q
//      |            |                    |                    |
//      +---+--------+-----------+--------+-----------+        |
//          |        |           |        |           |        |
//          |        V           |        V           |        V
//      +---+-------------+  +---+-------------+  +---+-------------+
//      | (2x16)x8 Lookup |  | (2x4)x27 Lookup |  | (2x4)x27 Lookup |
//      |       RAM       |  |       RAM       |  |       RAM       |
//      +--------+--------+  +--------+--------+  +--------+--------+
//               |                    |                    |
//               / 8                  / 8                  / 8
//               |                    |                    |
//      +--------+--------+  +--------+--------+  +--------+--------+
//      |>                |  |>                |  |>                |
//      +--------+--------+  +--------+--------+  +--------+--------+
//               |                    |                    |
//               / 8                  / 9                  / 9
//               |                    |                    |
//      +--------*-------------*------+---------------+    +--------+
//      |                      |      |               |             |
//      |      +---------------+------*---------------+------+      |
//      |      |               |      |               |      |      |
//      |      |      +--------+------+------+--------+------+------*
//      |      |      |        |      |      |        |      |      |
//    --+--   -+-   --+--    --+--   -+-   --+--    --+--   -+-   --+--
//     *    V     V    *      *    V     V    *      *    V     V    *
//      -------+-------        -------+-------        -------+-------
//             |                      |                      |
//             / 11                   / 11                   / 11
//             |                      |                      |
//      +------+------+        +------+------+        +------+------+
//      | Clamp 0-FF  |        | Clamp 0-FF  |        | Clamp 0-FF  |
//      +------+------+        +------+------+        +------+------+
//             |                      |                      |
//             / 8                    / 8                    / 8
//             |                      |                      |
//    +--------+--------+    +--------+--------+    +--------+--------+
//    |>                |    |>                |    |>                |
//    +--------+--------+    +--------+--------+    +--------+--------+
//             |                      |                      |
//             / 8 Red                / 8 Green              / 8 Blue
//             |                      |                      |
//             V 0.8                  V                      V
//

// 8-bit Palette
// The 8-bit palette is used for 8-bit P and 16-bit AP modes. The palette is loaded with register writes.
// During rendering, four texels are looked up simultaneously, each an independent 8-bit address.
// Palette Write

// The palette is written through the NCC table 0 I and Q register space when the MSB of the register write
// data is set. The NCC tables are not written when the I or Q NCC table register space is addressed and
// MSB of the register write data is set to 1 – Instead the data is stored in the texture palette.

//                          Palette Load Mechanism

// Register            LSB of P                Register Write Data
// Address
//                                       31                                      0
//                                       +---+--------+--------+--------+--------+
// nccTable0 I0        P[0]=0            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 I1        P[0]=1            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 I2        P[0]=0            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 I3        P[0]=1            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 Q0        P[0]=0            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 Q1        P[0]=1            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 Q2        P[0]=0            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+
// nccTable0 Q3        P[0]=1            | 1 | P[7:1] | R[7:0] | G[7:0] | B[7:0] |
//                                       +---+--------+--------+--------+--------+

// Note that the even addresses alias to the same location, as well as the odd ones. It is recommended that
// the table be written as 32 sets of 8 so that PCI bursts can be 8 transfers long.

typedef uint32_t NccTable_t;
namespace NccTable
{
    enum
    {
        Value = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

}