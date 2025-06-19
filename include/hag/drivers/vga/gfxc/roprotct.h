//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/regtype.h>
#include <hag/drivers/vga/gfxc/data.h>

namespace Hag::VGA::GraphicsController
{

namespace Register
{

enum
{
    RasterOperationRotateCount = 0x03,      //WT_ROP/RTC GR3
};

}

typedef uint8_t RasterOperationRotateCount_t;

//This register selects a raster operation function and indicates the number of bits the
//CPU data will be rotated (right) on the video memory write operation.

namespace RasterOperationRotateCount
{
    enum //Mask
    {
        RotateCount = 0x07,     //These bits define a binary encoded value of the number of
                                //positions to right-rotate data during a CPU memory write.
                                //To write non-rotated data, the CPU must preset a count of O.
        RasterOperation = 0x18, //The data written to memory can operate logically with the data
                                //already in the processor latches.
                                //This function is not available in write mode 1.
                                //The logical functions are defined as follows:
                                //00 = No operation
                                //01 = Logical AND with latched data
                                //10 = Logical OR with latched data
                                //11 = Logical XOR with latched data
                                //The logical function specified by this register is applied to
                                //data being written to memory while in modes 0, 2 and 3.
    };

    namespace Shift
    {
        enum
        {
            RotateCount = 0x00,
            RasterOperation = 0x03
        };
    }

    inline RasterOperationRotateCount_t Read()
    {
        GraphicsControllerIndex::Write(Register::RasterOperationRotateCount);
        return RasterOperationRotateCount_t(GraphicsControllerData::Read());
    }
    
    inline void Write(RasterOperationRotateCount_t value)
    {
        GraphicsControllerData::Write(Register::RasterOperationRotateCount, GraphicsControllerData_t(value));
    }

}

}
