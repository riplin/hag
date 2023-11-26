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
    ExtendedMiscellaneousControl1 = 0x66,                   //EXT-MISC-1 CR66
};

}

typedef uint8_t ExtendedMiscellaneousControl1_t;

namespace ExtendedMiscellaneousControl1
{

    enum //Mask
    {
        TriStateOffPixelAddressBus = 0x40,  //0 = Normal operation
                                            //1 = PA[15:0] are set to tri-state off
        PCIBusDisconnectEnable = 0x80       //0 = PCI bus disconnect disabled
                                            //1 = PCI bus disconnect enabled
                                            //Setting this bit to 1 allows PCI burst cycles to be interrupted
                                            //if AD[1:0] *- OOb or if the address during the burst goes outside
                                            //the address ranges supported by the Tri032ffri064.
    };

    namespace Shift
    {
        enum
        {
            TriStateOffPixelAddressBus = 0x06,
            PCIBusDisconnectEnable = 0x07
        };
    }

    inline ExtendedMiscellaneousControl1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedMiscellaneousControl1);
        return ExtendedMiscellaneousControl1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMiscellaneousControl1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedMiscellaneousControl1, VGA::CRTControllerData_t(value));
    }

}

}}}}
