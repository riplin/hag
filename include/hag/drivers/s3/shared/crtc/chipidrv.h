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
    ChipIDRevision = 0x30,                                  //CHIP-ID/REV CR30
};

}

typedef uint8_t ChipIDRevision_t;

namespace ChipIDRevision
{
    
    enum //Mask
    {
        RevisionStatus = 0x0F,
        ChipID = 0xF0               //When the software detects 0xE in ChipID field, it should then use CR2D, CR2E and
                                    //CR2F for chip ID information.
    };

    namespace Shift
    {
        enum
        {
            RevisionStatus = 0x00,
            ChipID = 0x04
        };
    }

    inline ChipIDRevision_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ChipIDRevision);
        return ChipIDRevision_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    //This is supposedly a read-only register, yet the BIOS is writing to it...
    inline void Write(VGA::Register_t controllerIndexRegister, ChipIDRevision_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ChipIDRevision, VGA::CRTControllerData_t(value));
    }

}

}}}}
