//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/matrox/shared/crtcext/regtype.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    CRTCExtensionIndex = 0x3DE,              //CRTCE
};

}

namespace CRTCExtensionIndex
{
    enum //Mask
    {
        CRTCERegisterIndex = 0xFF   //CRTCE ADDRESS
    };

    namespace Shift
    {
        enum
        {
            CRTCERegisterIndex = 0x00
        };
    }

    inline CRTCExtension::Register_t Read()
    {
        return CRTCExtension::Register_t(SYS_ReadPortByte(Register::CRTCExtensionIndex));
    }

    inline void Write(CRTCExtension::Register_t value)
    {
        SYS_WritePortByte(Register::CRTCExtensionIndex, value);
    }

}

}