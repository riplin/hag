//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/crtcext/index.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    CRTCExtensionData = 0x3DF,                              //CRTCE_DATA
};

}

typedef uint8_t CRTCExtensionData_t;

namespace CRTCExtensionData
{

    inline CRTCExtensionData_t Read()
    {
        return CRTCExtensionData_t(SYS_ReadPortByte(Register::CRTCExtensionData));
    }

    inline void Write(CRTCExtensionData_t value)
    {
        SYS_WritePortByte(Register::CRTCExtensionData, value);
    }
    
    inline void Write(CRTCExtension::Register_t crtExtIndex, CRTCExtensionData_t value)
    {
        SYS_WritePortBytes(Register::CRTCExtensionIndex, crtExtIndex, value);
    }
    
}

}