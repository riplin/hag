//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        SourceFormat = 0x054
    };
}

// srcFormat and dstFormat Registers
// These register specify the format and strides of the source and destination surfaces

// For linear surfaces, the stride of a pixel map is the number of bytes between the starting addresses of
// adjacent scan lines. For these surfaces, the units of the stride is always bytes, regardless of the pixel
// format.

// The number of bits per pixel is determined as described by the tables below. The ’32 bpp’ format contains
// 24 bits of RGB, along with a byte of unused data, the ’24 bpp’ is packed 24 bit color.

// Data coming through the host port can be byte swizzled to allow conversion between big and little endian
// formats, as selected by Bit 19 and 20 of src Format register. If both byte and word swizzling are enabled,
// the byte swizzling occurs first, followed by word swizzling.

// The source packing bits control how the stride of the source will be determined during blts. If both bits
// are zero, the stride is set by the stride entry. Otherwise, the stride is based off of the width of the blt being
// performed, as shown in the table below. The stride will equal the number of bytes in a row of the
// rectangle being blted plus as many bytes as are required to get the necessary alignment.

// For YUYV422 and UYVY422 source formats, linear strides must always be a dword multiple. Thus,
// bits(1:0) of the srcFormat register must be 0.

// When necessary, the blt engine will convert source pixels to the destination format.

// When source pixels in 15bpp or 16bpp format are converted to 24bpp or 32bpp, color conversion is
// performed by replicating the msbs of each channel into the extra lsbs required. When pixels are
// converted from 32bpp or 24bpp formats to 15 or 16bpp, 16bpp, the extra lsbs are removed from each
// channel. When any non-32bpp format is converted to 32bpp, the 8msbs of each pixel (i.e. the alpha
// channel) are filled with zeros.

// Destination pixel formats are stored as shown in the description of the colorFore and colorBack registers.
// RGB source formats match these, the other source formats are shown in the table below. For monochrome
// source, p0 represents the leftmost pixel on the screen and p31 represents the rightmost. For YUV formats,
// ya represents the left pixel and yb represents the pixel to the right of ya, etc. Thus, ya7 is the msb of the y
// channel for the left pixel and ya0 is the lsb of the y channel for that pixel. In the diagram, the dword with
// the lower address (which will be quadword aligned) is shown first, followed by the dword with the higher
// address.

//                                                          Monochrome
// +-------------------------------------------------------------------------------------------------------------------------------+
// |p24 p25 p26 p27 p28 p29 p30 p31 p16 p17 p18 p19 p20 p21 p22 p23  p8  p9 p10 p11 p12 p13 p14 p15  p0  p1  p2  p3  p4  p5  p6  p7|
// +-------------------------------------------------------------------------------------------------------------------------------+

//                                                          UYVY 4:2:2
// +-------------------------------------------------------------------------------------------------------------------------------+
// |yb7 yb6 yb5 yb4 yb3 yb2 yb1 yb0  v7  v6  v5  v4  v3  v2  v1  v0 ya7 ya6 ya5 ya4 ya3 ya2 ya1 ya0  u7  u6  u5  u4  u3  u2  u1  u0|
// +-------------------------------------------------------------------------------------------------------------------------------+

//                                                          YUYV 4:2:2
// +-------------------------------------------------------------------------------------------------------------------------------+
// | v7  v6  v5  v4  v3  v2  v1  v0 yb7 yb6 yb5 yb4 yb3 yb2 yb1 yb0  u7  u6  u5  u4  u3  u2  u1  u0 ya7 ya6 ya5 ya4 ya3 ya2 ya1 ya0|
// +-------------------------------------------------------------------------------------------------------------------------------+

// Methods of color translation used for Blts
//                  1bpp src        8bpp src        15bpp src       16bpp src       24bpp src       32bpp src       YUV src
// --------------------------------------------------------------------------------------------------------------------------------
//
// 8bpp dst         color           direct or       not supported   not supported   not supported   not supported   not supported
//                  registers       palette
// 
// 15bpp dst        color           not supported   direct          lsb removal     lsb removal     lsb removal,    YUV =>
//                  registers                                                                       alpha dropped   RGB
//
// 16bpp dst        color           not supported   msb             direct          lsb removal     lsb removal,    YUV =>
//                  registers                       duplication                                     alpha dropped   RGB
//
// 24bpp dst        color           not supported   msb             msb             direct          direct,         YUV =>
//                  registers                       duplication     duplication                     alpha dropped   RGB
//
// 32bpp dst        color           not supported   msb             msb             rgb direct,     direct          YUV =>
//                  registers                       duplication     duplication     zero alpha                      RGB,
//                                                                                                                  zero alpha

// srcPacking[23:22]    Packing                 Stride calculation
// 0                    Use stride register     srcFormat[13:0]
// 1                    Byte packed             ceil(src_width * src_bpp/8)
// 2                    Word packed             ceil(src_width * src_bpp/16)*2
// 3                    Double-word packed      ceil(src_width * src_bpp/32)*4

typedef uint32_t SourceFormat_t;
namespace SourceFormat
{

    enum
    {
        Stride =                        0x00003fff, // 13:0 Source Stride in bytes
                                                    // 15:14 RESERVED
        ColorFormat =                   0x000f0000, // 19:16 Source color format: 1, 8, 16, 24, 32 bpp RGB, YUYV422, UYVY422
        Format1Bpp =                    0x00000000,
        Format8BppPalettized =          0x00010000,
        Format16Bpp =                   0x00030000,
        Format24Bpp =                   0x00040000,
        Format32Bpp =                   0x00050000,
        FormatYUYV422 =                 0x00080000,
        FormatUYVY422 =                 0x00090000,
        HostPortByteSwizzle =           0x00100000, // 20 Host port byte swizzle (1=enable)
        HostPortWordSwizzle =           0x00200000, // 21 Host port word swizzle (1=enable)
        SourcePacking =                 0x00c00000, // 23:22 Source packing
        PackingStride =                 0x00000000, // Stride
        PackingByte =                   0x00400000, // Byte packed, ceil(src_width * src_bpp/8)
        PackingWord =                   0x00800000, // Word packed, ceil(src_width * src_bpp/16)*2
        PackingDoubleWord =             0x00c00000, // Double-word packed, ceil(src_width * src_bpp/32)*4
    };

    namespace Shift
    {
        enum
        {
            Stride = 0x00,
            ColorFormat = 0x10,
            HostPortByteSwizzle = 0x14,
            HostPortWordSwizzle = 0x15,
            SourcePacking = 0x16
        };
    }

}

}