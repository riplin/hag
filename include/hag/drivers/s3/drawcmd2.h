//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t DrawingCommand2_t;

namespace DrawingCommand2
{
    enum
    {
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
    };

    inline DrawingCommand2_t Read()
    {
        return DrawingCommand2_t(SYS_ReadPortShort(Register::DrawingCommand2));
    }

    inline void Write(DrawingCommand2_t value)
    {
        SYS_WritePortShort(Register::DrawingCommand2, value);
    }
}

}}
