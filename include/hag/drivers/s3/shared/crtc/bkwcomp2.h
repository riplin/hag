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
    BackwardCompatibility2 = 0x33,                          //BKWD_2 CR33
};

}

typedef uint8_t BackwardCompatibility2_t;

namespace BackwardCompatibility2
{
    
    enum //Mask
    {
        DisableVerticalDisplayEndExtensionBitsWriteProtection = 0x02,       //DIS VDE 0 = VDE protection enabled
                                                                            //        1 Disables the write protect setting
                                                                            //          of the bit 7 of CR11 on bits 1 and 6 of CR7
        VClockIsInternalDClock = 0x04,                                      //VCLK= DCK 0 = VCLK is the external VCLK (pass-through feature
                                                                            //          connector clock input enabled)
                                                                            //          or is divided by 2 for 4 bits/pixel modes
                                                                            //          (see bit 6 of AR10 or bit 4 of CR3A) or is
                                                                            //          the internal DCLK (if neither of the first two cases apply)
                                                                            //          1 = VCLK is the internal DCLK
        LockRAMDACWrites = 0x10,                                            // LOCK DACW 0 = Enable writes to RAMDAC registers
                                                                            //           1 = Disable writes to RAMDAC registers
        BlankBorderSelect = 0x20,                                           //BDR SEL 0 = ~BLANK active time is defined by CR2 and CR3
                                                                            //        1 = ~BLANK is active during entire display inactive period (no border)
        LockPaletteBorderColorRegisters = 0x40                              //LOCK PLTW 0 = Unlock Palette/Border Color registers
                                                                            //          1 = Lock Palette/Border Color registers
    };

    namespace Shift
    {
        enum
        {
            DisableVerticalDisplayEndExtensionBitsWriteProtection = 0x01,
            VClockIsInternalDClock = 0x03,
            LockRAMDACWrites = 0x04,
            BlankBorderSelect = 0x05,
            LockPaletteBorderColorRegisters = 0x06
        };
    }

    inline BackwardCompatibility2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::BackwardCompatibility2);
        return BackwardCompatibility2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, BackwardCompatibility2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::BackwardCompatibility2, VGA::CRTControllerData_t(value));
    }

    inline BackwardCompatibility2_t UnlockAll(VGA::Register_t controllerIndexRegister)
    {
        BackwardCompatibility2_t value = Read(controllerIndexRegister);
        Write(controllerIndexRegister, (value & BackwardCompatibility2_t(~(LockRAMDACWrites | LockPaletteBorderColorRegisters))) | BackwardCompatibility2_t(DisableVerticalDisplayEndExtensionBitsWriteProtection));
        return value & (LockRAMDACWrites | LockPaletteBorderColorRegisters | DisableVerticalDisplayEndExtensionBitsWriteProtection);
    }

    inline void LockAll(VGA::Register_t controllerIndexRegister, BackwardCompatibility2_t previousValue = DisableVerticalDisplayEndExtensionBitsWriteProtection)
    {
        BackwardCompatibility2_t value = Read(controllerIndexRegister);
        value &= BackwardCompatibility2_t(~(LockRAMDACWrites | LockPaletteBorderColorRegisters | DisableVerticalDisplayEndExtensionBitsWriteProtection));
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
        BackwardCompatibility2_t m_previousValue;
        VGA::Register_t m_crtcPort;
    };

}

}}}}
