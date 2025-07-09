//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        TriangleCommand = 0x080,
        FloatTriangleCommand = 0x100,
        NopCommand = 0x120,
        FastFillCommand = 0x124,
        SwapBufferCommand = 0x128,
        UserInterruptCommand = 0x13c,
        DrawTriangleCommand = 0x2a0,
        BeginTriangleCommand = 0x2a4,
    };
}

namespace Alias
{
    enum
    {
        TriangleCommand = 0x080,
        FloatTriangleCommand = 0x100,
    };
}

namespace Command
{

    // triangleCMD and ftriangleCMD Registers
    // The triangleCMD and ftriangleCMD registers execute the triangle drawing command. Writes to
    // triangleCMD or ftriangleCMD initiate rendering a triangle defined by the vertex, start, d?dX, and
    // d?dY registers. Note that the vertex, start, d?dX, and d?dY registers must be setup prior to writing to
    // triangleCMD or ftriangleCMD. The value stored to triangleCMD or ftriangleCMD is the area of the
    // triangle being rendered -- this value determines whether a triangle is clockwise or counter-clockwise
    // geometrically. If bit(31)=0, then the triangle is oriented in a counter-clockwise orientation (i.e. positive
    // area). If bit(31)=1, then the triangle is oriented in a clockwise orientation (i.e. negative area). To
    // calculate the area of a triangle, the following steps are performed:
    // 1.  The vertices (A, B, and C) are sorted by the Y coordinate in order of increasing Y (i.e. A.y <=
    //     B.y <= C.y)
    // 2.  The area is calculated as follows:
    //         AREA = ((dxAB * dyBC) - (dxBC * dyAB)) / 2
    //     where
    //         dxAB = A.x - B.x
    //         dyBC = B.y - C.y
    //         dxBC = B.x - C.x
    //         dyAB = A.y - B.y

    typedef uint32_t Triangle_t;
    namespace Triangle
    {
        enum
        {
            Sign = 0x80000000   // Sign of the area of the triangle to be rendered
        };

        namespace Shift
        {
            enum
            {
                Sign = 0x1f
            };
        }
    }

    // nopCMD Register
    // Writing any data to the nopCMD register executes the NOP command. Executing a NOP command
    // flushes the graphics pipeline. The lsb of the data value written to nopCMD is used to optionally clear the
    // fbiPixelsIn, fbiChromaFail, fbiZfuncFail, fbiAfuncFail, and fbiPixelsOut registers. Writing a ‘1’ to
    // the lsb of nopCMD will clear the aforementioned registers. Writing a ‘0’ to the lsb of nopCMD will not
    // modify the values of the aforementioned registers.

    typedef uint32_t Nop_t;
    namespace Nop
    {
        enum
        {
            Clear = 0x00000001  // Clear fbiPixelsIn, fbiChromaFail, fbiZfuncFail, fbiAfuncFail, and fbiPixelsOut
                                // registers (1=clear registers)
        };

        namespace Shift
        {
            enum
            {
                Clear = 0x00
            };
        }
    }

    // fastfillCMD Register
    // Writing any data to the fastfill register executes the FASTFILL command. The FASTFILL command is
    // used to clear the RGB and depth buffers as quickly as possible. Prior to executing the FASTFILL
    // command, the clipLeftRight and clipLowYHighY are loaded with a rectangular area which is the
    // desired area to be cleared. Note that clip registers define a rectangular area which is inclusive of the
    // clipLeft and clipLowY register values, but exclusive of the clipRight and clipHighY register values.
    // The fastfillCMD register is then written to initiate the FASTFILL command after the clip registers have
    // been loaded. FASTFILL will optionally clear the color buffers with the RGB color specified in the color1
    // register, and also optionally clears the depth buffer with the depth value taken from the zaColor register.
    // Note that since color1 is a 24-bit value, either dithering or bit truncation must be used to translate the 24-
    // bit value into the native 16-bit frame buffer -- dithering may be employed optionally as defined by bit(8) of
    // fbzMode. Disabling clearing of the color or depth buffers is accomplished by modifying the rgb/depth
    // mask bits(10:9) in fbzMode. This allows individual or combined clearing of the RGB and depth buffers.
    // Note that the fastfillCMD behaves differently when mctl_type_sdram=1(dramInit1[30]). When
    // fastfilling with SGRAMs (mctl_type_sdram=0), if dithering is enabled and fastfillCMD[0]=1, no
    // dithering will happen. But when fastfilling with SDRAMs (mctl_type_sdram=1), if dithering is enabled
    // and fastfillCMD[0]=1, dithering will still happen, since SDRAMs do not support blockwriting.

    typedef uint32_t FastFill_t;
    namespace FastFill
    {
        enum
        {
            NoDithering = 0x00000001    // Disable dithering during fastfill (1 = disable dithering).
        };

        namespace Shift
        {
            enum
            {
                NoDithering = 0x00
            };
        }
    }

    // swapbufferCMD Register
    // Writing to the swapbufferCMD register executes the SWAPBUFFER command. If the data written to
    // swapbufferCMD bit(0)=0, then the frame buffer swapping is not synchronized with vertical retrace. If
    // frame buffer swapping is not synchronized with vertical retrace, then visible frame “tearing” may occur.
    // If swapbufferCMD bit(0)=1 then the frame buffer swapping is synchronized with vertical retrace.
    // Synchronizing frame buffer swapping with vertical retrace eliminates the aforementioned frame “tearing.”
    // When a swapbufferCMD is received in the front-end PCI host FIFO, the swap buffers pending field in
    // the status register is incremented. Conversely, when an actual frame buffer swapping occurs, the swap
    // buffers pending field in the status register (bits(30:28)) is decremented. The swap buffers pending field
    // allows software to determine how many SWAPBUFFER commands are present in the Banshee FIFOs.
    // Bits(8:1) of swapbufferCMD are used to specify the number of vertical retraces to wait before swapping
    // the color buffers. An internal counter is incremented whenever a vertical retrace occurs, and the color
    // buffers are not swapped until the internal vertical retrace counter is greater than the value of
    // swapbufferCMD bits(8:1) -- After a swap occurs, the internal vertical retrace counter is cleared.
    // Specifying values other than zero for bits(8:1) are used to maintain constant frame rate.
    // SwapbufferCMD bit(9) disables swapping, which has the effect of decrementing the outstanding swap
    // count, but not performing a video pointer swap. Note that if vertical retrace synchronization is disabled for
    // swapping buffers (swapbufferCMD(0)=0), then the swap buffer interval field is ignored. The
    // swapbufferCMD on Banshee works similar to Voodoo Rush. The driver must write to the
    // swapbufferPend register to increase the outstanding swap count, then write to the swapbufferCMD
    // register.

    // NOTE: Bits 8:1 are zero-relative: i.e.: 0 means 1, 1 means 2, etc.

    // To enable triple buffering, turn on the appropriate bit in dram_init_1. If triple buffering is enabled, then
    // the graphics core will be allowed to continue given that one or fewer swaps is pending to be done by the
    // video unit. Effectively, this allows Banshee to render up to two frames ahead of the displayed buffer.

    typedef uint32_t SwapBuffer_t;
    namespace SwapBuffer
    {
        enum
        {
            VSync = 0x00000001,     // Synchronize frame buffer swapping to vertical retrace (1=enable)
            Interval = 0x0000001e,  // Swap buffer interval
            Disable = 0x00000020    // Swap buffer disable swap
        };

        namespace Shift
        {
            enum
            {
                VSync = 0x00,
                Interval = 0x01,
                Disable = 0x09
            };
        }
    }

    // userIntrCMD Register
    // Writing to the userIntrCMD register executes the USERINTERRUPT command:

    // If the data written to userIntrCMD bit(0)=0, then a user interrupt is generated (intrCtrl bit(11) is set to
    // 1). If the data written to userIntrCMD bit(1)=1, then the graphics engine stalls and waits for the
    // USERINTERRUPT interrupt to be cleared before continuing processing additional commands. If no
    // USERINTERRUPT interrupt is set and the data written to userIntrCMD bit(1)=1, then the graphics
    // engine will not stall and will continue to process additional commands. Software may also use
    // combinations of intrCtrl bits(1:0) to generate different functionality.

    // The tag associated with a user interrupt is written to userIntrCMD bits 9:2. When a user interrupt is
    // generated, the respective tag associated with the user interrupt is read from IntrCtrl bits 19:12.

    // If the USERINTERRUPT command does not stall the graphics engine (userIntrCMD(0)=1), then a
    // potential race condition occurs between multiple USERINTERRUPT commands and software user
    // interrupt processing. In particular, multiple USERINTERRUPT commands may be generated before
    // software is able to process the first interrupt. Irrespective of how many user interrupts have been
    // generated, the user interrupt tag field in intrCtrl (bits 19:12) always reflects the tag of last
    // USERINTERRUPT command processed. As a result of this behavior, early tags from multple
    // USERINTERRUPT commands may be lost. To avoid this behavior, software may force a single
    // USERINTERRUPT command to be executed at a time by writing userIntrCMD(1:0)=0x3 and cause the
    // graphics engine to stall until the USERINTERRUPT interrupt is cleared.

    // Note that bit 5 of intrCtrl must be set to 1 for user interrupts to be generated – writes to userIntrCMD
    // when intrCtrl(5)=0 do not generate interrupts or cause the processing of commands to wait on clearing of
    // the USERINTERRUPT command (regardless of the data written to userIntrCMD), and are thus in effect
    // “dropped.”

    typedef uint32_t UserInterrupt_t;
    namespace UserInterrupt
    {
        enum
        {
            StallUntilCleared = 0x00000001, // Wait for USERINTERRUPT to be cleared before continuing (1=stall graphics engine until interrupt is cleared)
            StallUntilGeneratedCleared = 0x00000002, // Wait for interrupt generated by USERINTERRUPT (visible in intrCtrl bit(11)) to be
                                                     // cleared before continuing (1=stall graphics engine until interrupt is cleared)
            Tag = 0x000003fc // User interrupt Tag
        };

        namespace Shift
        {
            enum
            {
                StallUntilCleared = 0x00,
                StallUntilGeneratedCleared = 0x01,
                Tag = 0x02
            };
        }
    }

    // sDrawTriCMD Register
    // The DrawTriCMD registers starts the draw process.

    typedef uint32_t DrawTriangle_t;
    namespace DrawTriangle
    {
        enum
        {
            Draw = 0x00000001  // Draw triangle
        };

        namespace Shift
        {
            enum
            {
                Draw = 0x00
            };
        }
    }

    // sBeginTriCMD Register
    // A write to this register begins a new triangle strip starting with the current vertex. No actual drawing is
    // performed.

    typedef uint32_t BeginTriangle_t;
    namespace BeginTriangle
    {
        enum
        {
            Begin = 0x00000001  // Begin New triangle
        };

        namespace Shift
        {
            enum
            {
                Begin = 0x00
            };
        }
    }

}

}