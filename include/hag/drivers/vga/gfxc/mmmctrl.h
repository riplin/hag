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
    MemoryMapModeControl = 0x06,            //MISC_GM GR6
};

}

typedef uint8_t MemoryMapModeControl_t;

//This register controls the video memory addressing.

namespace MemoryMapModeControl
{
    enum //Mask
    {
        SelectTextGraphicsMode = 0x01,  //0 = Text mode display addressing selected.
                                        //1 = Graphics mode display addressing selected.
                                        //    When set to graphics mode, the character
                                        //    generator address latches are disabled.
        ChainOddEvenPlanes = 0x02,      //0 = A0 address bit unchanged.
                                        //1 = CPU address bit AO is replaced by a higher order
                                        //    address bit. The content of A0 determines which
                                        //    memory plane is to be addressed. A0 = 0 selects
                                        //    planes 0 and 2, and A0 = 1 selects planes 1 and 3.
                                        //    This mode can be used to double the address space
                                        //    into video memory.
        MemoryMapMode = 0x0C,           //These bits control the address mapping of video memory
                                        //into the CPU address space.
        A0000HtoBFFFFH = 0x00,          //00 = AOOOOH to BFFFFH (128 KBytes)
        A0000HtoAFFFFH = 0x04,          //01 = AOOOOH to AFFFFH (64 KBytes)
        B0000HtoB7FFFH = 0x08,          //10 = BOOOOH to B7FFFH (32 KBytes)
        B8000HtoBFFFFH = 0x0C           //11 = B8000H to BFFFFH (32 KBytes)
    };

    namespace Shift
    {
        enum
        {
            SelectTextGraphicsMode = 0x00,
            ChainOddEvenPlanes = 0x01,
            MemoryMapMode = 0x2
        };
    }

    inline MemoryMapModeControl_t Read()
    {
        GraphicsControllerIndex::Write(Register::MemoryMapModeControl);
        return MemoryMapModeControl_t(GraphicsControllerData::Read());
    }
    
    inline void Write(MemoryMapModeControl_t value)
    {
        GraphicsControllerData::Write(Register::MemoryMapModeControl, GraphicsControllerData_t(value));
    }

}

}
