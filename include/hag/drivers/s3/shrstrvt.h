//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t ShortStrokeVectorTransfer_t;

namespace ShortStrokeVectorTransfer
{
    enum
    {
        PixelLength1 = 0x000F,          //Number of pixels - 1
        DrawPixel1 = 0x0010,            //0 = Move current position only - don't draw
                                        //1 = Draw pixel
        DrawingDirection1 = 0x00E0,     //Select Drawing Direction (measured counterclockwise from the X axis)
        Direction1Angle0 = 0x0000,      //000 = 0°
        Direction1Angle45 = 0x0020,     //001 = 45°
        Direction1Angle90 = 0x0040,     //010 = 90°
        Direction1Angle135 = 0x0060,    //011 = 135°
        Direction1Angle180 = 0x0080,    //100 = 180°
        Direction1Angle225 = 0x00A0,    //101 = 225°
        Direction1Angle270 = 0x00C0,    //110 = 270°
        Direction1Angle315 = 0x00E0,    //111 = 315°
        PixelLength2 = 0x0F00,          //Number of pixels - 1
        DrawPixel2 = 0x1000,            //0 = Move current position only - don't draw
                                        //1 = Draw pixel
        DrawingDirection2 = 0xE000,     //Select Drawing Direction (measured counterclockwise from the X axis)
        Direction2Angle0 = 0x0000,      //000 = 0°
        Direction2Angle45 = 0x2000,     //001 = 45°
        Direction2Angle90 = 0x4000,     //010 = 90°
        Direction2Angle135 = 0x6000,    //011 = 135°
        Direction2Angle180 = 0x8000,    //100 = 180°
        Direction2Angle225 = 0xA000,    //101 = 225°
        Direction2Angle270 = 0xC000,    //110 = 270°
        Direction2Angle315 = 0xE000,    //111 = 315°
    };

    inline ShortStrokeVectorTransfer_t Read()
    {
        return ShortStrokeVectorTransfer_t(SYS_ReadPortShort(Register::ShortStrokeVectorTransfer));
    }

    inline void Write(ShortStrokeVectorTransfer_t value)
    {
        SYS_WritePortShort(Register::ShortStrokeVectorTransfer, value);
    }
}

}}
