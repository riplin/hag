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
    SystemConfiguration = 0x40,                             //SYS_CNFG CR40
};

}

typedef uint8_t SystemConfiguration_t;

namespace SystemConfiguration
{
    enum //Mask
    {
        DisableEnhancedRegisterAccess = 0x00,   //Used to keep the code clean
        EnableEnhancedRegisterAccess = 0x01,    //0 = Enhanced register access disabled
                                                //1 = Enhanced register (x2E8H) access enabled
        ReadyControl = 0x10,                    // RDY CTL (VL-Bus only)
                                                // 0 = Minimum 0 wait state delay from ~SADS asserted to assertion of ~SRDY.
                                                //     Address latching occurs during the T1 cycle.
                                                // 1 = Minimum 1 wait state delay from ~SADS asserted to assertion of ~SRDY (Default)
                                                //     With this setting, bit 3 of CR58 determines when the address is latched.
        Reserved = 0x20,                        // Reserved value = 1
        UpperBitsMask = 0xF0
    };

    namespace Shift
    {
        enum
        {
            EnableEnhancedRegisterAccess = 0x00,
            ReadyControl = 0x04,
            Reserved = 0x05
        };
    }

    inline SystemConfiguration_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::SystemConfiguration);
        return SystemConfiguration_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, SystemConfiguration_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::SystemConfiguration, VGA::CRTControllerData_t(value));
    }

    inline SystemConfiguration_t Unlock(VGA::Register_t controllerIndexRegister)
    {
        SystemConfiguration_t value = Read(controllerIndexRegister);
        Write(controllerIndexRegister, value | EnableEnhancedRegisterAccess);
        return value & EnableEnhancedRegisterAccess;
    }

    inline void Lock(VGA::Register_t controllerIndexRegister, SystemConfiguration_t previousValue = DisableEnhancedRegisterAccess)
    {
        SystemConfiguration_t value = Read(controllerIndexRegister);
        value &= SystemConfiguration_t(~EnableEnhancedRegisterAccess);
        Write(controllerIndexRegister, value | previousValue);
    }

    class SoftUnlock
    {
    public:
        inline SoftUnlock(VGA::Register_t crtcPort)
            : m_previousValue(Unlock(crtcPort))
            , m_crtcPort(crtcPort)
        {
        }

        inline ~SoftUnlock()
        {
            Lock(m_crtcPort, m_previousValue);
        }

    private:
        SystemConfiguration_t m_previousValue;
        VGA::Register_t m_crtcPort;
    };

}

}
