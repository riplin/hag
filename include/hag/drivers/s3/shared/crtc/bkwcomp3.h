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
    BackwardCompatibility3 = 0x34,                          //BKWD_3 CR34
};

}

typedef uint8_t BackwardCompatibility3_t;

namespace BackwardCompatibility3
{
    
    enum //Mask
    {
        PCIDACSnoop = 0x01,                 //0 = Handling of PCI master aborts and retries during DAC cycles
                                            //    controlled by bits 1 and 2 of this register
                                            //1 = PCI master aborts and retries are not handled during DAC cycles
        PCIMasterAborts = 0x02,             //0 = PCI master aborts handled during DAC cycles
                                            //1 = PCI master aborts not handled during DAC cycles
                                            //Bit 0 of this register must be cleared to 0 for this bit to be effective.
                                            //
        PCIRetries = 0x04,                  //0 = PCI retries handled during DAC cycles
                                            //1 = PCI retries not handled during DAC cycles
                                            //Bit 0 of this register must be cleared to 0 for this bit to be effective.
                                            //
        EnableStartDisplayFIFOFetch = 0x10, //0 = Start Display FIFO Fetch register (CR3B) disabled
                                            //1 = Start Display FIFO Fetch register (CR3B) enabled
    };

    namespace Shift
    {
        enum
        {
            PCIDACSnoop = 0x00,
            PCIMasterAborts = 0x01,
            PCIRetries = 0x03,
            EnableStartDisplayFIFOFetch = 0x04,
        };
    }

    inline BackwardCompatibility3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::BackwardCompatibility3);
        return BackwardCompatibility3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, BackwardCompatibility3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::BackwardCompatibility3, VGA::CRTControllerData_t(value));
    }

}

}
