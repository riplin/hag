//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>


namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t RegisterLock2_t;

namespace RegisterLock2
{
    
    enum
    {
        UnlockCode = 0xA5,
        LockCode = 0x00
    };

    inline RegisterLock2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::RegisterLock2);
        return RegisterLock2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, RegisterLock2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::RegisterLock2, VGA::CRTControllerData_t(value));
    }

    inline void Unlock(VGA::Register_t controllerIndexRegister)
    {
        Write(controllerIndexRegister, UnlockCode);
    }

    inline void Lock(VGA::Register_t controllerIndexRegister, RegisterLock2_t previousValue = LockCode)
    {
        Write(controllerIndexRegister, previousValue);
    }

    template <VGA::Register_t CrtControllerIndex>
    class SoftUnlock
    {
        public:
        inline SoftUnlock()
            : m_previousValue(Read(CrtControllerIndex))
        {
            Unlock(CrtControllerIndex);
        }

        inline ~SoftUnlock()
        {
            Lock(CrtControllerIndex, m_previousValue);
        }

    private:
        RegisterLock2_t m_previousValue;
    };

}

}}}
