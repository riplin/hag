//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t Revision_t;

namespace Revision
{

    inline Revision_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::Revision);
        return Revision_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}}}
