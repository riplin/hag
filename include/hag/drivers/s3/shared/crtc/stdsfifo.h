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
    StartDisplayFIFO = 0x3B,                                //DT_EX_POS
};

}

typedef uint8_t StartDisplayFIFO_t;

namespace StartDisplayFIFO
{

    inline StartDisplayFIFO_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::StartDisplayFIFO);
        return StartDisplayFIFO_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, StartDisplayFIFO_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::StartDisplayFIFO, VGA::CRTControllerData_t(value));
    }

};

}}}}
