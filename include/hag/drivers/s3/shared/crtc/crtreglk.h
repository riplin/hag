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
    CRTRegisterLock = 0x35,                                 //CRTR_LOCK CR35
};

}

typedef uint8_t CRTRegisterLock_t;

namespace CRTRegisterLock
{
    
    enum //Mask
    {
        OldCPUBaseAddress = 0x0F,           //OLD-CPU-BASE-ADDRESS CPU Base Address Bits 17-14. These four bits define the CPU address base in 64 KByte
                                            //                     units of display memory. These bits are added with CPU address bit 17
                                            //                     (MSB of video memory addressing) to bit 14 for display buffer accesses.
                                            //                     Bits 3-2 of the ExtendedSystemControl2 register (CR51) are bits 19-18
                                            //                     of the address and enable access to up to 4 MBytes of display memory.
                                            //                     If a value is programmed into bits 5-0 of the ExtendedSystemControl4 register (CR6A),
                                            //                     this value becomes the upper 6 bits of the CPU base address and bits 3-0 of CR35
                                            //                     and bits 3-2 of CR51 are ignored. The Trio32 only supports 2MBytes, so the upper bit is not used.
        LockVerticalTimingRegisters = 0x10, //LOCK VTMG 0 = Vertical timing registers are unlocked
                                            //          1 = The following vertical timing registers are locked:
                                            //              CR6
                                            //              CR7 (bits 7, 5, 3, 2, 0)
                                            //              CR9 (bit 5)
                                            //              CR10
                                            //              CR11 (bits 3-0)
                                            //              CR15
                                            //              CR16
                                            //              CR6, CR7 are also locked by bit 7 of the VerticalRetraceEnd register (CR11).
        LockHorizontalTimingRegisters = 0x20//LOCK HTMG 0 = Horizontal timing registers are unlocked
                                            //          1 = The following horizontal timing registers are locked:
                                            //              CR00
                                            //              CR1
                                            //              CR2
                                            //              CR3
                                            //              CR4
                                            //              CR5
                                            //              CR17 (bit 2)
                                            //              All these registers (except bit 2 of CR17) are also locked by bit 7 of the 
                                            //              VerticalRetraceEnd register (CR11).
    };

    namespace Shift
    {
        enum
        {
            OldCPUBaseAddress = 0x00,
            LockVerticalTimingRegisters = 0x04,
            LockHorizontalTimingRegisters = 0x05
        };
    }

    inline CRTRegisterLock_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::CRTRegisterLock);
        return CRTRegisterLock_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, CRTRegisterLock_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::CRTRegisterLock, VGA::CRTControllerData_t(value));
    }

    inline CRTRegisterLock_t UnlockAll(VGA::Register_t controllerIndexRegister)
    {
        CRTRegisterLock_t value = Read(controllerIndexRegister);
        Write(controllerIndexRegister, value & CRTRegisterLock_t(~(LockVerticalTimingRegisters | LockHorizontalTimingRegisters)));
        return value & CRTRegisterLock_t(LockVerticalTimingRegisters | LockHorizontalTimingRegisters);
    }

    inline void LockAll(VGA::Register_t controllerIndexRegister, CRTRegisterLock_t previousValue = LockVerticalTimingRegisters | LockHorizontalTimingRegisters)
    {
        CRTRegisterLock_t value = Read(controllerIndexRegister);
        value &= CRTRegisterLock_t(~(LockVerticalTimingRegisters | LockHorizontalTimingRegisters));
        Write(controllerIndexRegister, value | previousValue);
    }

    class SoftUnlock
    {
    public:
        inline SoftUnlock(VGA::Register_t crtcPort)
            : m_previousValue(UnlockAll(crtcPort))
            , m_crtcPort(crtcPort)
        {
        }

        inline ~SoftUnlock()
        {
            LockAll(m_crtcPort, m_previousValue);
        }

    private:
        CRTRegisterLock_t m_previousValue;
        VGA::Register_t m_crtcPort;
    };

}

}
