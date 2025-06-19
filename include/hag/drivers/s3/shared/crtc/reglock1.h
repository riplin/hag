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
    RegisterLock1 = 0x38,                                   //REG_LOCK1 CR38
};

}

typedef uint8_t RegisterLock1_t;

namespace RegisterLock1
{
    
    enum
    {
        UnlockCode = 0x48,
        LockCode = 0x00
    };

    inline RegisterLock1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::RegisterLock1);
        return RegisterLock1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, RegisterLock1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::RegisterLock1, VGA::CRTControllerData_t(value)); }

    inline void Unlock(VGA::Register_t controllerIndexRegister)
    {
        Write(controllerIndexRegister, UnlockCode);
    }

    inline void Lock(VGA::Register_t controllerIndexRegister, RegisterLock1_t previousValue = LockCode)
    {
        Write(controllerIndexRegister, previousValue);
    }

    class SoftUnlock
    {
    public:
        inline SoftUnlock(VGA::Register_t crtcPort)
            : m_previousValue(Read(crtcPort))
            , m_crtcPort(crtcPort)
        {
            Unlock(m_crtcPort);
        }

        inline ~SoftUnlock()
        {
            Lock(m_crtcPort, m_previousValue);
        }

    private:
        RegisterLock1_t m_previousValue;
        VGA::Register_t m_crtcPort;
    };

}

}
