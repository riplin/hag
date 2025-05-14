//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/matrox/shared/crtc/regtype.h>

namespace Hag { namespace Matrox { namespace Shared { namespace CRTController
{
    
namespace Register
{

enum
{
    CPUReadLatch = 0x22,                                 //CR22
};
    
}

//This register reads one of four 8-bit registers of the graphics controller CPU data latch.
//These latches are loaded when the CPU reads from display memory. The rdmapsl
//field (GCTL4<1:0>) determines which of the four planes is read in Read Mode 0. This
//register contains color compare data in Read Mode 1.

typedef VGA::CRTControllerData_t CPUReadLatch_t;

namespace CPUReadLatch
{

    inline CPUReadLatch_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::CPUReadLatch);
        return CPUReadLatch_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, CPUReadLatch_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::CPUReadLatch, VGA::CRTControllerData_t(value));
    }

}

}}}}