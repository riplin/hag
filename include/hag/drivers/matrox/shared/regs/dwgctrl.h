//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    DrawingControl = 0x1C00,
    DrawingControlCommit = 0x1D00
};

}

// 3 3 2 2222 2 2222 1111 1 1 1 1 1 1
// 1 0 9 8765 4 3210 9876 5 4 3 2 1 098 7 654 3210
// |      |     |    |    |       |     |     |   |
// 8 4 2 1842 1 8421 8421 8 4 2 1 8 421 8 421 8421
// x 0 0 0000 x 0000 0000 x 0 0 0 0 000 0 000 0000
//   | |    |      |    |   | | | |   | |   |    |
//   | |    |      |    |   | | | |   | |   |    +--- opcod
//   | |    |      |    |   | | | |   | |   |
//   | |    |      |    |   | | | |   | |   +-------- atype
//   | |    |      |    |   | | | |   | |
//   | |    |      |    |   | | | |   | +------------ linear
//   | |    |      |    |   | | | |   |
//   | |    |      |    |   | | | |   +-------------- zmode
//   | |    |      |    |   | | | |
//   | |    |      |    |   | | | +------------------ solid
//   | |    |      |    |   | | |
//   | |    |      |    |   | | +-------------------- arzero
//   | |    |      |    |   | |
//   | |    |      |    |   | +---------------------- sgnzero
//   | |    |      |    |   |
//   | |    |      |    |   +------------------------ shftzero
//   | |    |      |    |
//   | |    |      |    +---------------------------- bop
//   | |    |      |
//   | |    |      +--------------------------------- trans
//   | |    |
//   | |    +---------------------------------------- bltmod
//   | |
//   | +--------------------------------------------- pattern
//   |
//   +----------------------------------------------- transc

typedef uint32_t DrawingControl_t;

namespace DrawingControl
{

    enum //Mask
    {
        Opcode =                    0x0000000F, // Operation code. The Opcode field defines the operation that is selected by the drawing engine.
        LineOpen =                  0x00000000, // 
        AutoLineOpen =              0x00000001, // AUTO
        LineClose =                 0x00000002, // WRITE LAST
        AutoLineClose =             0x00000003, // AUTO, WRITE LAST
        Trapezoid =                 0x00000004, //
        TrapezoidILoad =            0x00000005, // Data from host
        //Texture =                  0x00000006, // Texture (undocumented)
        ILoadHiqh =                 0x00000007, // HOST -> RAM scale, high quality filter
        BitBlt =                    0x00000008, // RAM -> RAM
        ILoad =                     0x00000009, // HOST -> RAM
        IDump =                     0x0000000A, // RAM -> HOST
        ILoadScale =                0x0000000D, // HOST -> RAM scale
        ILoadHighv =                0x0000000E, // HOST -> RAM horizontal and vertical scale, high quality filter
        ILoadFilter =               0x0000000F, // HOST -> RAM scale, filter

        AccessType =                0x00000070, // Access type. The atype field is used to define the type of access performed to the RAM.
        AccessReplace =             0x00000000, // RPL, Write (replace)
        AccesRaster =               0x00000010, // RSTR, Read-modify-write (raster)
        DepthGouraud =              0x00000030, // ZI, Depth mode with Gouraud
        BlockWrite =                0x00000040, // BLK, Block write mode 
                                                //      When block mode is selected, only RPL operations can be performed. Even if
                                                //      the bop field is programmed to a different value, RPL will be used.
                                                //      The hardpwmsk field of the OPTION register must be set to ‘1’.
        Gouraud =                   0x00000070, // I, Gouraud (with depth compare)
                                                //    Depth comparison works according to the zmode setting (same as ‘ZI’);
                                                //    however, the depth is never updated.

        LinearMode =                0x00000080, // Linear mode. Specifies whether the blit is linear or xy.
        XYBlit =                    0x00000000, // 0 = xy blit
        LinearBlit =                0x00000080, // 1 = linear blit

        ZMode =                     0x00000700, // The z drawing mode. This field must be valid for drawing using depth. This field
                                                // specifies the type of comparison to use.
        ZAlways =                   0x00000000, // 000 = NOZCMP
        ZEqual =                    0x00000200, // 010 = ZE     When depth is =
        ZNotEqual =                 0x00000300, // 011 = ZNE    When depth is <>
        ZLessThan =                 0x00000400, // 100 = ZLT    When depth is <
        ZLessThanEqual =            0x00000500, // 101 = ZLTE   When depth is <=
        ZGreaterThan =              0x00000600, // 110 = ZGT    When depth is >
        ZGreaterThanEqual =         0x00000700, // 111 = ZGTE   When depth is >=

        Solidity =                  0x00000800, // Solid line or constant trapezoid. The solid register is not a physical register. It provides
                                                // an alternate way to load the SRC registers (see page 4-73).
                                                // Setting solid is useful for line drawing with no linestyle, or for trapezoid drawing with
                                                // no patterning. It forces the color expansion circuitry to provide the foreground color
                                                // during a line or a trapezoid drawing. Writing to any of the SRC0, SRC1, SRC2,
                                                // SRC3 or PAT0, PAT1 registers while solid is ‘1’ may produce unpredicatable
                                                // results.
        Patterned =                 0x00000000, // 0 = No effect
        Solid =                     0x00000800, // 1 = SRC0 <= FFFFFFFFh
                                                //     SRC1 <= FFFFFFFFh
                                                //     SRC2 <= FFFFFFFFh
                                                //     SRC3 <= FFFFFFFFh

        ARZero =                    0x00001000, // AR register at zero. The ARSet field provides an alternate way to set certain AR
                                                // registers (see descriptions starting on page 4-20).
                                                // Setting ARSet is useful when drawing rectangles, and also for certain blit operations.
                                                // In the case of rectangles (Trapezoid Opcode):
                                                //      dYl <= 0 (AR0)
                                                //      errl <= 0 (AR1)
                                                //      -|dXl| <= 0 (AR2)
                                                //      errr <= 0 (AR4)
                                                //      -|dXr| <= 0 (AR5)
                                                //      dYr <= 0 (AR6)
                                                // Writing to the ARx registers when ARSet = 1 will produce unpredictable results.
                                                // 0 = no effect
                                                // 1 = AR0 <= 0h
                                                //     AR1 <= 0h
                                                //     AR2 <= 0h
                                                //     AR4 <= 0h
                                                //     AR5 <= 0h
                                                //     AR6 <= 0h

        SignZero =                  0x00002000, // Sign register at zero. The SignZero bit provides an alternate way to set all the fields in
                                                // the Sign register.
                                                // 0 = No effect
                                                // 1 = SGN <= 0h
                                                // Setting SignZero is useful during TRAP and some blit operations.
                                                // For TRAP:    scanleft = 0 Horizontal scan right
                                                //              sdxl = 0 Left edge in increment mode
                                                //              sdxr = 0 Right edge in increment mode
                                                //              sdy = 0 iy (see PITCH on page 4-68) is added to
                                                //              ydst (see YDST on page 4-80)
                                                // For BLIT:    scanleft = 0 Horizontal scan right
                                                //              sdxl = 0 Left edge in increment mode
                                                //              sdxr = 0 Right edge in increment mode
                                                //              sdy = 0 iy is added to ydst
                                                // Writing to the Sign register when SignZero = 1 will produce unpredictable results.

        ShiftZero =                 0x00004000, // Shift register at zero. The ShiftZero bit provides an alternate way to set all the fields of
                                                // the FunnelShifterControl register.
                                                // 0 = No effect
                                                // 1 = SHIFT <= 0h

        BooleanOperation =          0x000F0000, // Boolean operation between a source and a destination slice. The table below shows
                                                // the various functions performed by the Boolean ALU for 8, 16, 24 and, 32 bits/pixel.
                                                // During block mode operations, bop must be set to BoolSrc (0xC).
                                                // BooleanOperation         Function
        BoolZero =                  0x00000000, //      0000                    0
        BoolDstOrSrcNot =           0x00010000, //      0001                ~(D | S)
        BoolDstAndNotSrc =          0x00020000, //      0010                 D & ~S
        BoolNotSrc =                0x00030000, //      0011                   ~S
        BoolNotDstAndSrc =          0x00040000, //      0100                ~(D) & S
        BoolNotDst =                0x00050000, //      0101                   ~D
        BoolDstXorSrc =             0x00060000, //      0110                  D ^ S
        BoolDstAndSrcNot =          0x00070000, //      0111                ~(D & S)
        BoolDstAndSrc =             0x00080000, //      1000                  D & S
        BoolDstXorSrcNot =          0x00090000, //      1001                ~(D ^ S)
        BoolDst =                   0x000A0000, //      1010                    D
        BoolDstOrNotSrc =           0x000B0000, //      1011                  D | ~S
        BoolSrc =                   0x000C0000, //      1100                    S
        BoolNotDstOrSrc =           0x000D0000, //      1101               (~D) | S
        BoolDstOrSrc =              0x000E0000, //      1110                  D | S
        BoolOne =                   0x000F0000, //      1111                    1

        Translucidity =             0x00F00000, // Translucidity. Specify the percentage of opaqueness of the object. The opaqueness is
                                                // realized by writing one of ‘n’ pixels. The trans field specifies the following transparency
                                                // pattern (where 'X' squares are opaque and ' ' squares are transparent):
                                                //
                                                //  0000   0001   0010   1111   0011   0100   0101   0110
                                                // +----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+
                                                // |XXXX| |X X | | X X| |    | |X X | | X X| |    | |    |
                                                // |XXXX| | X X| |X X | |    | |    | |    | |X X | | X X|
                                                // |XXXX| |X X | | X X| |    | |X X | | X X| |    | |    |
                                                // |XXXX| | X X| |X X | |    | |    | |    | |X X | | X X|
                                                // +----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+
                                                //
                                                //  0111   1000   1001   1010   1011   1100   1101   1110
                                                // +----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+
                                                // |X   | |    | |   X| |    | |    | | X  | |    | |  X |
                                                // |    | | X  | |    | |  X | |X   | |    | |   X| |    |
                                                // |  X | |    | | X  | |    | |    | |   X| |    | |X   |
                                                // |    | |   X| |    | |X   | |  X | |    | | X  | |    |
                                                // +----+ +----+ +----+ +----+ +----+ +----+ +----+ +----+

        BlitMode =                  0x1E000000, // Blit mode selection. This field is defined as used during BLIT and ILOAD operations.
                                                // For line drawing with line style, this field must have the value BlitFormattedColor
                                                // in order to handle the line style properly.
                                                // For a RAM-to-RAM BITBLT operation, hardware fast clipping will be enabled if
                                                // BlitFormattedColor is specified.
                                                // The field is also used for the IDUMP and TRAP_ILOAD operations.
                                                // Refer to the subsections contained in ‘Drawing in Power Graphic Mode’ on page 5-25
                                                // for more information on how to use this field. That section also presents the definition
                                                // of the various pixel formats.
        BlitMonoLittleEndian =      0x00000000, // 0000 = Source operand is monochrome in 1 bpp.
                                                //        For ILOAD, the source data is in little endian format.
        BlitMonoWindowsFormat =     0x08000000, // 0100 = Source operand is monochrome in 1 bpp.
                                                //        For ILOAD, the source data is in Windows format.
        BlitMonoPlane =             0x02000000, // 0001 = Source operand is monochrome from one plane.
        BlitFormattedColor =        0x04000000, // 0010 = Source operand is color.
                                                //        Source is formatted when it comes from host.
        Blit422YUV =                0x1C000000, // 1110 = Source operand is color.
                                                //        For ILOAD, the source data is in 4:2:2 YUV format.
        Blit32BGR =                 0x06000000, // 0011 = Source operand is color.
                                                //        For ILOAD, the source data is in 32 bpp, BGR format.
        Blit32RGB =                 0x0E000000, // 0111 = Source operand is color.
                                                //        For ILOAD, the source data is in 32 bpp, RGB format.
        Blit24BGR =                 0x16000000, // 1011 = Source operand is color.
                                                //        For ILOAD, the source data is in 24 bpp, BGR format.
        Blit24RGB =                 0x1E000000, // 1111 = Source operand is color.
                                                //        For ILOAD, the source data is in 24 bpp, RGB format.

        PatternEnable =             0x20000000, // Patterning enable. This bit specifies if the patterning is enabled when performing
                                                // BITBLT operations.

        TransparencyEnable =        0x40000000  // Transparency color enabled. This field can be enabled for blits, vectors that have a
                                                // linestyle, and trapezoids with patterning. For operations with color expansion, this bit
                                                // specifies if the background color is used.
                                                // 0 = Background color is opaque.
                                                // 1 = Background color is transparent.
                                                // For other types of blit, this field enables the transparent blit feature, based on a com-
                                                // parison with a transparent color key. This transparency is defined by the following
                                                // equation:
                                                // if ( transc==1 && (source & bltcmsk==bltckey) )
                                                //     do not update the destination
                                                // else
                                                //     update the destination with the source
                                                // Refer to the FCOL and BCOL register descriptions for the definitions of the bltckey
                                                // and bltcmsk fields, respectively.
    };

    namespace Shift
    {
        enum
        {
            Opcode = 0x00,
            AccessType = 0x04,
            LinearMode = 0x07,
            ZMode = 0x08,
            Solidity = 0x0B,
            ARZero = 0x0C,
            SignZero = 0x0D,
            ShiftZero = 0x0E,
            BooleanOperation = 0x10,
            Translucidity = 0x14,
            BlitMode = 0x19,
            PatternEnable = 0x1D,
            TransparencyEnable = 0x1E
        };
    }
}

}}}
