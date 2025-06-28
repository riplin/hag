//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        DestinationFormat = 0x014
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

typedef uint32_t DestinationFormat_t;
namespace DestinationFormat
{

    enum
    {
        Stride = 0x00003fff,    // Destination Stride in bytes
        Bpp = 0x00070000,       // Destination bits per pixel: 8, 15, 16, 24, or 32
        Bpp8 = 0x00010000,      // 
        Bpp16 = 0x00030000,      // 
        Bpp24 = 0x00040000,      // 
        Bpp32 = 0x00050000,      // 
    };

    namespace Shift
    {
        enum
        {
            Stride = 0x00,
            Bpp = 0x10
        };
    }
}

}