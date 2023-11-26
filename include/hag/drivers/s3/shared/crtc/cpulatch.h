//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    CPULatchData = 0x22,                                    //GCCL CR22
};

}

//This register is used to read the CPU latch in the Graphics Controller.
//GRAPHICS CONTROLLER CPU LATCH - N
//Bits 1-0 of GR4 select the latch number N (3-0) of the CPU Latch.

typedef uint8_t CPULatchData_t;

namespace CPULatchData
{

    inline CPULatchData_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::CPULatchData);
        return CPULatchData_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}}}}
