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
    Revision = 0x2F,                                        //CR2F
};

}

typedef uint8_t Revision_t;

namespace Revision
{

    inline Revision_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Revision);
        return Revision_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}
