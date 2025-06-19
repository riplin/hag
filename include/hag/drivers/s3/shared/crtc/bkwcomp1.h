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
    BackwardCompatibility1 = 0x32,                          //BKWD_1 CR32
};

}

typedef uint8_t BackwardCompatibility1_t;

namespace BackwardCompatibility1
{
    
    enum //Mask
    {
        InterruptEnable = 0x10,         //0 = All interrupt generation disabled
                                        //1 = Interrupt generation enabled
        StandardVGAMemoryMapping = 0x40,//VGA FXPG - Use Standard VGA Memory Wrapping
                                        //0 = Memory accesses extending past a 256K boundary do not wrap
                                        //1 = Memory accesses extending past a 256K boundary wrap at the boundary
                                        //The standard 256K VGA memory page always ends on a natural 256K boundary
                                        //and accesses beyond this boundary will wrap. If the starting address is moved
                                        //via bits 4-0 of CR69 (or bits 5-4 of CR31 and bits 1-0 of CR51). the 256K page
                                        //may not end on a 256K boundary and accesses past the boundary will not wrap.
                                        //This is the case when this bit is cleared to O. For standard VGA compatibility
                                        //when the page base address is moved. this bit is set to 1 to cause wrapping at
                                        //a 256K boundary.
    };

    namespace Shift
    {
        enum
        {
            InterruptEnable = 0x04,
            StandardVGAMemoryMapping = 0x06,
        };
    }

    inline BackwardCompatibility1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::BackwardCompatibility1);
        return BackwardCompatibility1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, BackwardCompatibility1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::BackwardCompatibility1, VGA::CRTControllerData_t(value));
    }

}

}
