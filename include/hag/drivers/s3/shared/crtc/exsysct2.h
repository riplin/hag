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
    ExtendedSystemControl2 = 0x51,                          //EX_SCTL_2 CR51
};

}

typedef uint8_t ExtendedSystemControl2_t;

namespace ExtendedSystemControl2
{
    
    enum //Mask
    {
        OldDisplayStartAddressHigh = 0x03,      //OLD-DSAD These are the extension bits of MemoryConfiguration register (CR31) bits 5-4
                                                //         (Display Start Base Address). If the upper 4 display start address bits are
                                                //         programmed into bits 3-0 of CR69, these bits and bits 5-4 are ignored.
                                                //         The Trio32 only supports 2 MBytes of memory, so the upper bit is not used.
        OldCPUBaseAddressBitsHigh = 0x0C,       //OLD-CBAD These are extension bits of CRTRegisterLock register (CR35) bits 3-0 (CPU Base Address).
                                                //         They become bits 19-18 of the CPU base address, enabling access to up to 4 Mbytes
                                                //         of display memory. If the upper 6 CPU base address bits are programmed into
                                                //         bits 5-0 of CR6A, these bits and bits 3-0 of CR35 are ignored. The Trio32
                                                //         only supports 2 MBytes of memory, so the upper bit is not used.
        LogicalScreenWidthHigh = 0x30           //LOG-SCR-W These are two extension bits of the Offset register (CR13).
                                                //          If the  value of these bits is not 00b, bit 2 of the Extended Mode register (CR43) is disabled.
    };

    namespace Shift
    {
        enum
        {
            OldDisplayStartAddressHigh = 0x00,
            OldCPUBaseAddressBitsHigh = 0x02,
            LogicalScreenWidthHigh = 0x04
        };
    }

    inline ExtendedSystemControl2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedSystemControl2);
        return ExtendedSystemControl2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedSystemControl2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedSystemControl2, VGA::CRTControllerData_t(value));
    }

}

}}}}
