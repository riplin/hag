//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag::S3::Shared::CRTController
{

namespace Register
{

enum
{
    ExtendedMemoryControl1 = 0x53,                          //EXT_MCTL_1 CR53
};

}

typedef uint8_t ExtendedMemoryControl1_t;

namespace ExtendedMemoryControl1
{

    enum //Mask
    {
        EnableWritePerBit = 0x01,   //0 = Disable write per bit for all memory banks
                                    //1 = Enable write per bit for all memory banks
        EnableMMIOAccess = 0x010,   //0 = Disable (Default)
                                    //1 = Enable Memory-Mapped I/O
                                    //Refer to the MMIO explanation in Section 13 for more information.
                                    //
        SwapNibbles = 0x40          //0 = No nibble swap
                                    //1 = Swap nibbles in each byte of a linear memory address read or write operation
    };

    namespace Shift
    {
        enum
        {
        EnableWritePerBit = 0x00,
        EnableMMIOAccess = 0x004,
        SwapNibbles = 0x06
        };
    }

    inline ExtendedMemoryControl1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedMemoryControl1);
        return ExtendedMemoryControl1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMemoryControl1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedMemoryControl1, VGA::CRTControllerData_t(value));
    }

}

}
