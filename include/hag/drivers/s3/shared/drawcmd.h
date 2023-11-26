//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{

enum
{
    DrawingCommand = 0x9AE8,                                //CMD
};

}

typedef uint16_t DrawingCommand_t;

namespace DrawingCommand
{
    enum
    {
        MustBe1 = 0x0001,                   //This bit must always be programmed to 1.
        AcrossThePlanePixelMode = 0x0002,   //0 = Single pixel transferred at a time
                                            //1 = Multiple pixels transferred at a time (across the plane mode)
        LastPixelOff = 0x0004,              //0 = Last pixel of line or vector draw will be drawn
                                            //1 = Last pixel of line or vector draw will not be drawn
        SelectRadialDirectionType = 0x0008, //0 = x-y (axial)
                                            //1 = Radial
        DrawPixel = 0x0010,                 //0 = Move the current position only - don't draw
                                            //1 = Draw pixel(s)
        SelectDrawingDirection = 0x00E0,    //In the following table, radial drawing angle is measured counterclockwise
                                            //from the X axis. For axial line draws, the line is drawn from left to right
                                            //or a +X and from right to left for a -X, down for a +Y and up for a -Yo X
                                            //or Y maj specifies the longest axis.
                                            //      Radial (bit 3 = 1)      x-y (Axial - bit 3 = 0)
        XMajor = 0x0000,
        YMajor = 0x0040,
        NegX = 0x0000,
        PosX = 0x0020,
        NegY = 0x0000,
        PosY = 0x0080,
        NegYXMajNegX = NegY | XMajor | NegX,
        Radial0deg = NegYXMajNegX,          //000           0°                      -Y, X maj, -X
        NegYXmajPosX = NegY | XMajor | PosX,
        Radial45deg = NegYXmajPosX,         //001           45°                     -Y, X maj, +X
        NegYYmajNegX = NegY | YMajor | NegX,
        Radial90deg = NegYYmajNegX,         //010           90°                     -Y, Y maj, -X
        NegYYmajPosX = NegY | YMajor | PosX,
        Radial135deg = NegYYmajPosX,        //011           135°                    -Y, Y maj, +X
        PosYXmajNegX = PosY | XMajor | NegX,
        Radial180deg = PosYXmajNegX,        //100           180°                    +Y, X maj, -X
        PosYXmajPosX = PosY | XMajor | PosX,
        Radial225deg = PosYXmajPosX,        //101           225°                    +Y, X maj, +X
        PosYYmajNegX = PosY | YMajor | NegX,
        Radial270deg = PosYYmajNegX,        //110           270°                    +Y, Y maj, -X
        PosYYmajPosX = PosY | YMajor | PosX,
        Radial315deg = PosYYmajPosX,        //111           315°                    +Y, Y maj, +X

        WaitForCPUData = 0x0100,            //0 = Use Graphics Engine-based data
                                            //1 = Wait for data to be transferred to or from the CPU through the E2E8H port
        BusSize = 0x0600,                   //Select image write (E2E8H, E2EAH) bus transfer width
        Bus8bit = 0x0000,                   //00 = 8 bits
        Bus16bit = 0x0200,                  //01 = 16 bits
        Bus32bit = 0x0400,                  //10 = 32 bits - All doubleword bits beyond the image rectangle width are discarded.
                                            //               Each line starts with a fresh doubleword. The current drawing position 
                                            //               ends up one pixel below the lower left hand corner of the image rectangle.
        Bus32bitAcrossThePlane = 0x0600,    //11 = 32 bits - (Trio32 8, 16 and 32 bits/pixel modes only) This setting applies only to
                                            //               image transfers across the plane (each bit transferred is converted to a 
                                            //               pixel). Only bits from the end of the line width to the next byte boundary
                                            //               are discarded. Data for the next line begins with the next byte. The current
                                            //               drawing position ends up one pixel to the right of the top right corner of
                                            //               the image rectangle.
                                            //This parameter applies only to writing data through the Pixel Data Transfer (E2E8H, E2EAH)
                                            //registers (programmed I/O or memory-mapped I/O).
                                            //
        EnableByteSwap = 0x1000,            //0 = High byte first, low byte second
                                            //1 = Low byte first, high byte second
        CommandType = 0xE800,               //
        CommandNop = 0x0000,                //This is used to set up short stroke vector drawing without writing a pixel.
        CommandDrawLine = 0x2000,           //If bit 3 of this register is cleared to 0, the axial step constant,
                                            //diagonal step constant and error term are used to draw the line. 
                                            //If bit 3 is set to 1, the line will be drawn at the angle specified by bits 7-5 and
                                            //with a length in pixels as specified by the Major Axis Pixel Count (96E8H) register.
        CommandRectangleFill = 0x4000,      //The position, width and height of a rectangle are defined.
                                            //The rectangle is filled with a solid color.
        CommandPolygonFillSolid = 0x6000,   //(Tri064 only) The starting and ending coordinates for 2 lines of an n-sided polygon
                                            //(n = 3 or more) are defined. Only the ending coordinates are required for subsequent
                                            //commands required to complete the polygon. The polygon is filled with a single color.
        Command4PointTrapezoidFillSolid = 0x8000,   //(Tri064 only) This is a special case of the polygon fill solid command, with 
                                                    //the top and bottom edges required to be horizontal.
        CommandBresenhamParameterTrapezoidalFillSolid = 0xA000, //(Tri064 only) The starting positions and Bresenham parameters
                                                                //(axial and diagonal step constants and error terms) are defined
                                                                //for two lines that form the sides of a trapezoid. The top and bottom
                                                                //lines must be horizontal. The trapezoid is filled with a single color.
        CommandBitBLT = 0xC000,             //A rectangle of defined location, width and height is moved to another defined
                                            //location in display memory.
        CommandPatBLT = 0xE000,             //An 8x8 pixel patterned rectangle of defined location is transferred repeatedly to a
                                            //destination rectangle of defined location, width and height. The pattern copy is always
                                            //aligned to an 8 pixel boundary and transfers continue until the pattern is tiled into the
                                            //entire destination rectangle. The starting X coordinate of the source pattern rectangle
                                            //should always be on an 8 pixel boundary.
        CommandPolyLine2PointLine = 0x2800, //(Tri064 only) The starting and ending coordinates for a line segment are defined. For
                                            //subsequent polyline (connected) segments, only the ending point need be programmed.
                                            //If only a single segment is drawn, this is called a 2-point line operation.
        CommandPolygonFillPattern = 0x6800, //(Tri064 only) Same as the Polygon Fill Solid except that the polygon is filled with
                                            //an 8x8 pixel pattern whose location is specified in the Pattern Y (EAE8H) and 
                                            //Pattern X (EAEAH) registers.
        Command4PointTrapezoidFillPattern = 0x8800, //(Tri064 only) Same as the 4-point Trapezoidal Fill Solid except that the
                                                    //trapezoid is filled with an 8x8 pixel pattern whose location is specified in
                                                    //the Pattern Y (EAE8H) and Pattern X (EAEAH) registers.
        CommandBresenhamParameterTrapezoidalFillPattern = 0xA800,   //(Tri064 only) Same as the Bresenham Parameter Trapezoidal
                                                                    //Fill Solid except that the trapezoid is filled with an 8x8
                                                                    //pixel pattern whose location is specified in the Pattern Y
                                                                    //(EAE8H) and Pattern X (EAEAH) registers.
    };

    inline DrawingCommand_t Read()
    {
        return DrawingCommand_t(SYS_ReadPortShort(Register::DrawingCommand));
    }

    inline void Write(DrawingCommand_t value)
    {
        SYS_WritePortShort(Register::DrawingCommand, value);
    }
}

namespace MMIO
{

    namespace DrawingCommand
    {
        inline void Write(DrawingCommand_t value)
        {
            *FARPointer(0xA000, Register::DrawingCommand).
                ToPointer<DrawingCommand_t>(sizeof(DrawingCommand_t)) = value;
        }
    }

}

}}}
