//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        LinearFrameBufferMode = 0x114
    };
}

// lfbMode Register
// The lfbMode register controls linear frame buffer accesses.

// The following table shows the supported Banshee linear frame buffer write formats:

// Value       Linear Frame Buffer Write Format
//             16-bit formats
// 0           16-bit RGB (5-6-5)
// 1           16-bit RGB (x-5-5-5)
// 2           16-bit ARGB (1-5-5-5)
// 3           Reserved
//             32-bit formats
// 4           24-bit RGB (x-8-8-8)
// 5           32-bit ARGB (8-8-8-8)
// 7:6         Reserved
// 11:8        Reserved
// 12          16-bit depth, 16-bit RGB (5-6-5)
// 13          16-bit depth, 16-bit RGB (x-5-5-5)
// 14          16-bit depth, 16-bit ARGB (1-5-5-5)
// 15          16-bit depth, 16-bit depth

// When accessing the linear frame buffer, the cpu accesses information from the starting linear frame buffer
// (LFB) address space (see section 4 on Banshee address space) plus an offset which determines the <x,y>
// coordinates being accessed. Bits(3:0) of lfbMode define the format of linear frame buffer writes.

// When writing to the linear frame buffer, lfbMode bit(8)=1 specifies that LFB pixels are processed by the
// normal Banshee pixel pipeline -- this implies each pixel written must have an associated depth and alpha
// value, and is also subject to the fog mode, alpha function, etc. If bit(8)=0, pixels written using LFB access
// bypass the normal Banshee pixel pipeline and are written to the specified buffer unconditionally and the
// values written are unconditionally written into the color/depth buffers except for optional color dithering
// [depth function, alpha blending, alpha test, and color/depth write masks are all bypassed when bit(8)=0].
// If bit(8)=0, then only the buffers that are specified in the particular LFB format are updated. Also note
// that if lfbMode bit(8)=0 that the color and Z mask bits in fbzMode(bits 9 and 10) are ignored for LFB
// writes. For example, if LFB modes 0-2, or 4 are used and bit(8)=0, then only the color buffers are updated
// for LFB writes (the depth buffer is unaffected by all LFB writes for these modes, regardless of the status of
// the Z-mask bit fbzMode bit 10). However, if LFB modes 12-14 are used and bit(8)=0, then both the color
// and depth buffers are updated with the LFB write data, irrespective of the color and Z mask bits in
// fbzMode. If LFB mode 15 is used and bit(8)=0, then only the depth buffer is updated for LFB writes (the
// color buffers are unaffected by all LFB writes in this mode, regardless of the status of the color mask bits
// in fbzMode).

// If lfbMode bit(8)=0 and a LFB write format is selected which contains an alpha component (formats 2, 5,
// and 14) and the alpha buffer is enabled, then the alpha component is written into the alpha buffer.
// Conversely, if the alpha buffer is not enabled, then the alpha component of LFB writes using formats 2, 5,
// and 14 when bit(8)=0 are ignored. Note that anytime LFB formats 2, 5, and 14 are used when bit(8)=0
// that blending and/or chroma-keying using the alpha component is not performed since the pixel-pipeline
// is bypassed when bit(8)=0.

// If lfbMode bit(8)=0 and LFB write format 14 is used, the component that is ignored is determined by
// whether the alpha buffer is enabled -- If the alpha buffer is enabled and LFB write format 14 is used with
// bit(8)=0, then the depth component is ignored for all LFB writes. Conversely, if the alpha buffer is
// disabled and LFB write format is used with bit(8)=0, then the alpha component is ignored for all LFB
// writes.

// If lfbMode bit(8)=1 and a LFB write access format does not include depth or alpha information (formats
// 0-5), then the appropriate depth and/or alpha information for each pixel written is taken from the zaColor
// register. Note that if bit(8)=1 that the LFB write pixels are processed by the normal Banshee pixel
// pipeline and thus are subject to the per-pixel operations including clipping, dithering, alpha-blending,
// alpha-testing, depth-testing, chroma-keying, fogging, and color/depth write masking.

// Bits(10:9) of lfbMode specify the RGB channel format (color lanes) for linear frame buffer writes. The
// table below shows the Banshee supported RGB lanes:

// Value       RGB Channel Format
// 0           ARGB
// 1           ABGR
// 2           RGBA
// 3           BGRA

// Bit(11) of lfbMode defines the format of 2 16-bit data types passed with a single 32-bit writes. For linear
// frame buffer formats 0-2, two 16-bit data transfers can be packed into one 32-bit write -- bit(11) defines
// which 16-bit shorts correspond to which pixels on screen. The table below shows the pixel packing for
// packed 32-bit linear frame buffer formats 0-2:


// lfbMode bit(11)     Screen Pixel Packing
// 0                   Right Pixel(host data 31:16), Left Pixel(host data 15:0)
// 1                   Left Pixel(host data 31:16), Right Pixel(host data 15:0)

// For linear frame buffer formats 12-14, bit(11) of lfbMode defines the bit locations of the 2 16-bit data
// types passed. The table below shows the data packing for 32-bit linear frame buffer formats 12-14:

// lfbMode bit(11)     Screen Pixel Packing
// 0                   Z value(host data 31:16), RGB value(host data 15:0)
// 1                   RGB value(host data 31:16), Z value(host data 15:0)

// Note that bit(11) of lfbMode is ignored for linear frame buffer writes using formats 4 or 5.

// Bit(12) of lfbMode is used to enable byte swizzling. When byte swizzling is enabled, the 4-bytes within a
// 32-bit word are swizzled to correct for endian differences between Banshee and the host CPU. For little
// endian CPUs (e.g. Intel x86 processors) byte swizzling should not be enabled, however big endian CPUs
// (e.g. PowerPC processors) should enable byte swizzling. For linear frame buffer writes, the bytes within a
// word are swizzled prior to being modified by the other control bits of lfbMode. When byte swizzling is
// enabled, bits(31:24) are swapped with bits(7:0), and bits(23:16) are swapped with bits(15:8).

// Very Important Note: The order of swapping and swizzling operations for LFB writes is as follows: byte
// swizzling is performed first on all incoming LFB data, as defined by lfbMode bit(12) and irrespective of
// the LFB data format. After byte swizzling, 16-bit word swapping is performed as defined by lfbMode
// bit(11). Note that 16-bit word swapping is never performed on LFB data when data formats 4 and 5 are
// used. Also note that 16-bit word swapping is performed on the LFB data that was previously optionally
// swapped. Finally, after both swizzling and 16-bit word swapping are performed, the individual color
// channels are selected as defined in lfbMode bits(10:9). Note that the color channels are selected on the
// LFB data that was previously swizzled and/or swapped

// Bit(13) of lfbMode is used to define the origin of the Y coordinate for all linear frame buffer writes when
// the pixel pipeline is bypassed (lfbMode bit(8)=0). Note that bit(13) of lfbMode does not affect rendering
// operations (FASTFILL and TRIANGLE commands) -- bit(17) of fbzMode defines the origin of the Y
// coordinate for rendering operations. Note also that if the pixel pipeline is enabled for linear frame buffer
// writes (lfbMode bit(8)=1), then fbzMode bit(17) is used to determine the location of the Y origin. When
// cleared, the Y origin (Y=0) for all linear frame buffer accesses is defined to be at the top of the screen.
// When bit(13) is set, the Y origin for all linear frame buffer accesses is defined to be at the bottom of the
// screen.

// Bit(14) of lfbMode is used to select the W component used for LFB writes processed through the pixel
// pipeline. If bit(14)=0, then the MSBs of the fractional component of the 48-bit W value passed to the
// pixel pipeline for LFB writes through the pixel pipeline is the 16-bit Z value associated with the LFB
// write. [Note that the 16-bit Z value associated with the LFB write is dependent on the LFB format, and is
// either passed down pixel-by-pixel from the CPU, or is set to the constant zaColor(15:0)]. If bit(14)=1,
// then the MSBs of the fractional component of the 48-bit W value passed to the pixel pipeline for LFB
// writes is zacolor(15:0). Regardless of the setting of bit(14), when LFB writes go through the pixel
// pipeline, all other bits except the 16 MSBs of the fractional component of the W value are set to 0x0.
// Note that bit(14) is ignored if LFB writes bypass the pixel pipeline.

// Linear Frame Buffer Writes


// Linear frame buffer writes -- format 0:
// When writing to the linear frame buffer with 16-bit format 0 (RGB 5-6-5), the RGB channel format
// specifies the RGB ordering within a 16-bit word. If the Banshee pixel pipeline is enabled for LFB
// accesses (lfbMode bit(8)=1), then alpha and depth information for LFB format 0 is taken from the
// zaColor register. The following table shows the color channels for 16-bit linear frame buffer access
// format 0:

// RGB Channel         16-bit Linear frame         RGB Channel
// Format Value        buffer access bits
// 0                       15:0                    Red (15:11), Green(10:5), Blue(4:0)
// 1                       15:0                    Blue (15:11), Green(10:5), Red(4:0)
// 2                       15:0                    Red (15:11), Green(10:5), Blue(4:0)
// 3                       15:0                    Blue (15:11), Green(10:5), Red(4:0)


// Linear frame buffer writes -- format 1:
// When writing to the linear frame buffer with 16-bit format 1 (RGB 5-5-5), the RGB channel format
// specifies the RGB ordering within a 16-bit word. If the Banshee pixel pipeline is enabled for LFB
// accesses (lfbMode bit(8)=1), then alpha and depth information for LFB format 1 is taken from the
// zaColor register. The following table shows the color channels for 16-bit linear frame buffer access
// format 1:

// RGB Channel         16-bit Linear frame         RGB Channel
// Format Value        buffer access bits
// 0                       15:0                    Ignored(15), Red (14:10), Green(9:5), Blue(4:0)
// 1                       15:0                    Ignored(15), Blue (14:10), Green(9:5), Red(4:0)
// 2                       15:0                    Red (15:11), Green(10:6), Blue(5:1), Ignored(0)
// 3                       15:0                    Blue (15:11), Green(10:6), Red(5:1), Ignored(0)


// Linear frame buffer writes -- format 2:
// When writing to the linear frame buffer with 16-bit format 2 (ARGB 1-5-5-5), the RGB channel format
// specifies the RGB ordering within a 16-bit word. If the Banshee pixel pipeline is enabled for LFB
// accesses (lfbMode bit(8)=1), then depth information for LFB format 2 is taken from the zaColor register.
// Note that the 1-bit alpha value passed when using LFB format 2 is bit-replicated to yield the 8-bit alpha
// used in the pixel pipeline. The following table shows the color channels for 16-bit linear frame buffer
// access format 2:

// RGB Channel         16-bit Linear frame         RGB Channel
// Format Value        buffer access bits
// 0                           15:0                Alpha(15), Red (14:10), Green(9:5), Blue(4:0)
// 1                           15:0                Alpha(15), Blue (14:10), Green(9:5), Red(4:0)
// 2                           15:0                Red (15:11), Green(10:6), Blue(5:1), Alpha(0)
// 3                           15:0                Blue (15:11), Green(10:6), Red(5:1), Alpha(0)


// Linear frame buffer writes -- format 3:
// Linear frame buffer format 3 is an unsupported format.


// Linear frame buffer writes -- format 4:
// When writing to the linear frame buffer with 24-bit format 4 (RGB x-8-8-8), the RGB channel format
// specifies the RGB ordering within a 24-bit word. Note that the alpha/A channel is ignored for 24-bit
// access format 4. Also note that while only 24-bits of data is transfered for format 4, all data access must
// be 32-bit aligned -- packed 24-bit writes are not supported by Banshee. If the Banshee pixel pipeline is
// enabled for LFB accesses (lfbMode bit(8)=1), then alpha and depth information for LFB format 4 is taken
// from the zaColor register. The following table shows the color channels for 24-bit linear frame buffer
// access format 4:

// RGB Channel         24-bit Linear frame         RGB Channel
// Format Value        buffer access bits
//                     (aligned to 32-bits)
// 0                           31:0                Ignored(31:24), Red (23:16), Green(15:8), Blue(7:0)
// 1                           31:0                Ignored(31:24), Blue(23:16), Green(15:8), Red(7:0)
// 2                           31:0                Red(31:24), Green(23:16), Blue(15:8), Ignored(7:0)
// 3                           31:0                Blue(31:24), Green(23:16), Red(15:8), Ignored(7:0)


// Linear frame buffer writes -- format 5:
// When writing to the linear frame buffer with 32-bit format 5 (ARGB 8-8-8-8), the RGB channel format
// specifies the ARGB ordering within a 32-bit word. If the Banshee pixel pipeline is enabled for LFB
// accesses (lfbMode bit(8)=1), then depth information for LFB format 5 is taken from the zaColor register.
// The following table shows the color channels for 32-bit linear frame buffer access format 5.

// RGB Channel         32-bit Linear frame         RGB Channel
// Format Value        buffer access bits
//                     (aligned to 32-bits)
// 0                           31:0                    Alpha(31:24), Red (23:16), Green(15:8), Blue(7:0)
// 1                           31:0                    Alpha(31:24), Blue(23:16), Green(15:8), Red(7:0)
// 2                           31:0                    Red(31:24), Green(23:16), Blue(15:8), Alpha(7:0)
// 3                           31:0                    Blue(31:24), Green(23:16), Red(15:8), Alpha(7:0)


// Linear frame buffer writes -- formats 6-11:
// Linear frame buffer formats 6-11 are unsupported formats.

// Linear frame buffer writes -- format 12:
// When writing to the linear frame buffer with 32-bit format 12 (Depth 16, RGB 5-6-5), the RGB channel
// format specifies the RGB ordering within the 32-bit word. If the Banshee pixel pipeline is enabled for
// LFB accesses (lfbMode bit(8)=1), then alpha information for LFB format 12 is taken from the zaColor
// register. Note that the format of the depth value passed when using LFB format 12 must precisely match
// the format of the type of depth buffering being used (either 16-bit integer Z or 16-bit floating point 1/W).
// The following table shows the 16-bit color channels within the 32-bit linear frame buffer access format
// 12:

// RGB Channel         16-bit Linear frame         RGB Channel
// Format Value        buffer access bits
// 0                           15:0                Red (15:11), Green(10:5), Blue(4:0)
// 1                           15:0                Blue (15:11), Green(10:5), Red(4:0)
// 2                           15:0                Red (15:11), Green(10:5), Blue(4:0)
// 3                           15:0                Blue (15:11), Green(10:5), Red(4:0)


// Linear frame buffer writes -- format 13:
// When writing to the linear frame buffer with 32-bit format 13 (Depth 16, RGB x-5-5-5), the RGB channel
// format specifies the RGB ordering within the 32-bit word. If the Banshee pixel pipeline is enabled for
// LFB accesses (lfbMode bit(8)=1), then alpha information for LFB format 13 is taken from the zaColor
// register. Note that the format of the depth value passed when using LFB format 13 must precisely match
// the format of the type of depth buffering being used (either 16-bit integer Z or 16-bit floating point 1/W).
// The following table shows the 16-bit color channels within the 32-bit linear frame buffer access format
// 13:

// RGB Channel         16-bit Linear frame         RGB Channel
// Format Value        buffer access bits
// 0                           15:0                Ignored(15), Red (14:10), Green(9:5), Blue(4:0)
// 1                           15:0                Ignored(15), Blue (14:10), Green(9:5), Red(4:0)
// 2                           15:0                Red (15:11), Green(10:6), Blue(5:1), Ignored(0)
// 3                           15:0                Blue (15:11), Green(10:6), Red(5:1), Ignored(0)


// Linear frame buffer writes -- format 14:
// When writing to the linear frame buffer with 32-bit format 14 (Depth 16, ARGB 1-5-5-5), the RGB
// channel format specifies the RGB ordering within the 32-bit word. Note that the format of the depth
// value passed when using LFB format 14 must precisely match the format of the type of depth buffering
// being used (either 16-bit integer Z or 16-bit floating point 1/W). Also note that the 1-bit alpha value
// passed when using LFB format 14 is bit-replicated to yield the 8-bit alpha used in the pixel pipeline. The
// following table shows the 16-bit color channels within the 32-bit linear frame buffer access format 14:

// RGB Channel         16-bit Linear frame         RGB Channel
// Format Value        buffer access bits
// 0                           15:0                Alpha(15), Red (14:10), Green(9:5), Blue(4:0)
// 1                           15:0                Alpha(15), Blue (14:10), Green(9:5), Red(4:0)
// 2                           15:0                Red (15:11), Green(10:6), Blue(5:1), Alpha(0)
// 3                           15:0                Blue (15:11), Green(10:6), Red(5:1), Alpha(0)


// Linear frame buffer writes -- format 15:
// When writing to the linear frame buffer with 32-bit format 15 (Depth 16, Depth 16), the format of the
// depth values passed must precisely match the format of the type of depth buffering being used (either 16-
// bit integer Z or 16-bit floating point 1/W). If the Banshee pixel pipeline is enabled for LFB accesses
// (lfbMode bit(8)=1), then RGB color information is taken from the color1 register, and alpha information
// for LFB format 15 is taken from the zaColor register.


typedef uint32_t LinearFrameBufferMode_t;
namespace LinearFrameBufferMode
{
    enum
    {
        Format =                        0x0000000f, // 3:0 Linear frame buffer write format (see table below)
        Format16BppRGB =                0x00000000,
        Format15BppRGB =                0x00000001,
        Format16BppARGB =               0x00000002,
        Format24BppRGB =                0x00000004,
        Format32BppARGB =               0x00000005,
        Format16BppZ16BppRGB =          0x0000000c,
        Format16BppZ15BppRGB =          0x0000000d,
        Format16BppZ16BppARGB =         0x0000000e,
        Format16BppZ16BppZ =            0x0000000f,
        
        PixelPipelineProcessing =       0x00000100, // 8 Enable Banshee pixel pipeline-processed linear frame buffer writes (1=enable)
        PipelineProcessingDisabled =    0x00000000,
        PipelineProcessingEnabled =     0x00000100,

        RGBALanes =                     0x00000600, // 10:9 Linear frame buffer RGBA lanes (see tables below)
        LaneARGB =                      0x00000000,
        LaneABGR =                      0x00000200,
        LaneRGBA =                      0x00000400,
        LaneBGRA =                      0x00000600,

        WordSwap =                      0x00000800, // 11 16-bit word swap linear frame buffer writes (1=enable)
        SwapDisabled =                  0x00000000,
        SwapEnabled =                   0x00000800,

        ByteSwizzle =                   0x00001000, // 12 Byte swizzle linear frame buffer writes (1=enable)
        SwizzleDisabled =               0x00000000,
        SwizzleEnabled =                0x00001000,

        YOrigin =                       0x00002000, // 13 LFB access Y origin (0=top of screen is origin, 1=bottom of screen is origin)
        YOriginTop =                    0x00000000,
        YOriginBottom =                 0x00002000,

        WriteAccessWSelect =            0x00004000, // 14 Linear frame buffer write access W select (0=LFB selected, 1=zacolor[15:0]).
        SelectLFB =                     0x00000000,
        SelectZAColor =                 0x00004000
    };

    namespace Shift
    {
        enum
        {
            Format =                        0x00,
            PixelPipelineProcessing =       0x08,
            RGBALanes =                     0x09,
            WordSwap =                      0x0b,
            ByteSwizzle =                   0x0c,
            YOrigin =                       0x0d,
            WriteAccessWSelect =            0x0e
        };
    }
}

}