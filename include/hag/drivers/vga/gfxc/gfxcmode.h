//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/gfxc/regtype.h>
#include <hag/drivers/vga/gfxc/data.h>

namespace Hag { namespace VGA { namespace GraphicsController
{

namespace Register
{

enum
{
    GraphicsControllerMode = 0x05,          //GRP_MODE GR5
};

}

typedef uint8_t GraphicsControllerMode_t;

namespace GraphicsControllerMode
{
    enum //Mask
    {
        SelectWriteMode = 0x03,         //These bits select the CPU write mode into video memory.
                                        //The function of each mode is defined as follows:
        Mode0 = 0x00,                   //00 = Write Mode o. Each of four video memory planes is
                                        //     written with the CPU data rotated by the number of 
                                        //     counts in the rotate register. If the Set/Reset
                                        //     register is enabled for any of four planes, the
                                        //     corresponding plane is written with the data stored
                                        //     in the set/reset register. Raster operations and bit
                                        //     mask registers are effective.
        Mode1 = 0x01,                   //01 = Write Mode 1. Each of four video memory planes is written
                                        //     with the data in the processor latches. These latches are
                                        //     loaded during previous CPU read opera- tions. Raster
                                        //     operation, rotate count, set/reset data, enable
                                        //     set/reset data and bit mask registers are not effective.
        Mode2 = 0x02,                   //10 = Write Mode 2. Memory planes 0-3 are filled with B bits of
                                        //     the value of CPU write data bits 0-3, respectively.
                                        //     For example, if write data bit 0 is a 1, eight 1's are
                                        //     written to memory plane O. The data on the CPU data bus is
                                        //     treated as the color value. The Bit Mask register is
                                        //     effective as the Mask register. A logical 1 in the
                                        //     Bit Mask register sets the corresponding pixel in the
                                        //     addressed byte to the color specified on the data bus. A
                                        //     logical 0 in the Bit Mask register sets the corresponding
                                        //     pixel in the addressed byte to the corresponding pixel in the
                                        //     processor latches. The Set/Reset, Enable Set/Reset and
                                        //     Rotate Count registers are ignored.
        Mode3 = 0x03,                   //11 = Write Mode 3. Each of four video memory planes is written with
                                        //     B bits of the color value contained in the set/reset register
                                        //     for that plane. The Enable Set/ Reset register is not effective.
                                        //     Rotated CPU write data is ANDed with the bit mask register to
                                        //     form an 8-bit value that performs the same function as the
                                        //     Bit Mask register in write modes 0 and 2. This write mode can
                                        //     be used to fill an area with a single color and pattern.
        EnableReadCompare = 0x08,       //0 =  The CPU reads data from the video memory planes.
                                        //     The plane is selected by the Read Plane Select register.
                                        //     This is called read mode 0.
                                        //1 =  The CPU reads the results of the logical comparison between the
                                        //     data in four video memory planes selected by the contents of
                                        //     the Color Don't Care register and the contents of the
                                        //     Color Compare register. The result is a 1 for a match and 0 for
                                        //     a mismatch on each pixel. This is called read mode 1.
        OddEvenAddressing = 0x10,       //0 =  Standard addressing.
                                        //1 =  Odd/even addressing mode selected. Even CPU addresses access
                                        //     plane 0 and 2, while odd CPU addresses access plane 1 and 3.
                                        //     This option is useful for emulating the CGA compatible mode.
                                        //     The value of this bit should be the inverted value programmed
                                        //     in bit 2 of the Sequencer Memory Mode register (SR4). This bit
                                        //     affects reading of display memory by the CPU.
        SelectEvenOddShiftMode = 0x20,  //0 =  Normal shift mode.
                                        //1 =  The video shift registers in the graphics section are directed
                                        //     to format the serial data stream with even-numbered bits from
                                        //     both planes on the even-numbered planes and odd-numbered bits
                                        //     from both planes on the odd planes.
        Select256ColorShiftMode = 0x40  //0 =  Bit 5 in this register controls operation ofthe video
                                        //     shift registers.
                                        //1 =  The shift registers are loaded in a manner that supports the
                                        //     256 color mode.
    };

    namespace Shift
    {
        enum
        {
            SelectWriteMode = 0x03,
            EnableReadCompare = 0x08,
            OddEvenAddressing = 0x10,
            SelectEvenOddShiftMode = 0x20,
            Select256ColorShiftMode = 0x40
        };
    }

    inline GraphicsControllerMode_t Read()
    {
        GraphicsControllerIndex::Write(Register::GraphicsControllerMode);
        return GraphicsControllerMode_t(GraphicsControllerData::Read());
    }
    
    inline void Write(GraphicsControllerMode_t value)
    {
        GraphicsControllerData::Write(Register::GraphicsControllerMode, GraphicsControllerData_t(value));
    }

}

}}}
