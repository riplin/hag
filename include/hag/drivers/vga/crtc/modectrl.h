//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t CRTCModeControl_t;

namespace CRTCModeControl
{
    enum //Mask
    {
        Bank2Mode = 0x01,           //0 = Row scan counter bit 0 is substituted for memory address bit 13
                                    //    during active display time.
                                    //1 = Memory address bit 13 appears on the memory address output bit 13
                                    //    signal of the CRT controller.
                                    //This bit allows memory mapping compatibility with the IBM CGA graphics mode.
        Bank4Mode = 0x02,           //0 = Row scan counter bit 1 is substituted for memory address bit 14 during
                                    //    active display time.
                                    //1 = Memory address bit 14 appears on the memory address output bit 14
                                    //    signal of the CRT controller.
                                    //The combination of this bit and bit 0 of this register allows compatibility
                                    //with Hercules HGC graphics memory mapping.
        VerticalTotalDouble = 0x04, //0 = Horizontal retrace clock selected.
                                    //1 = Horizontal retrace clock divided by two selected.
                                    //This bit selects horizontal retrace clock or horizontal retrace clock divided
                                    //by two as the clock that controls the vertical timing counter. If the vertical
                                    //retrace counter is clocked with the horizontal retrace clock divided by 2,
                                    //then the vertical resolution is double.
        WordMode = 0x08,            //0 = Memory address counter is clocked with the character clock input,
                                    //    and byte mode addressing for the video memory is selected.
                                    //1 = Memory address counter is clocked by the character clock input
                                    //    divided by 2, and word mode addressing for the video memory is selected.
        AddressWrap16K = 0x20,      //0 = When word mode is selected by bit 6 of this register, memory address
                                    //    counter bit 13 appears on the memory address output bit 0 signal of the
                                    //    CRT controller and the video memory address wraps around at 16 KBytes.
                                    //1 = When word mode is selected by bit 6 of this register, memory address
                                    //    counter bit 15 appears on the memory address output bit 0 signal of
                                    //    the CRT controller.
                                    //This bit is useful in implementing IBM CGA mode.
        ByteMode = 0x40,            //0 = Word mode shifts all memory address counter bits down one bit,
                                    //    and the most significant bit of the counter appears on the least
                                    //    significant bit of the memory address output.
                                    //1 = Byte address mode.
        HardwareReset = 0x80        //0 = Vertical and horizontal retrace pulses always inactive.
                                    //1 = Vertical and horizontal retrace pulses enabled.
                                    //This bit does not reset any other registers or outputs.
    };

    namespace Shift
    {
        enum
        {
            Bank2Mode = 0x00,
            Bank4Mode = 0x01,
            VerticalTotalDouble = 0x02,
            WordMode = 0x03,
            AddressWrap16K = 0x05,
            ByteMode = 0x06,
            HardwareReset = 0x07
        };
    }

    inline CRTCModeControl_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::CRTCModeControl);
        return CRTCModeControl_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, CRTCModeControl_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::CRTCModeControl, CRTControllerData_t(value));
    }

    inline uint1_t GetBank2Mode(CRTCModeControl_t value)
    {
        return (value & Bank2Mode) >> Shift::Bank2Mode;
    }

    inline uint1_t GetBank4Mode(CRTCModeControl_t value)
    {
        return (value & Bank4Mode) >> Shift::Bank4Mode;
    }

    inline uint1_t GetVerticalTotalDouble(CRTCModeControl_t value)
    {
        return (value & VerticalTotalDouble) >> Shift::VerticalTotalDouble;
    }

    inline uint1_t GetWordMode(CRTCModeControl_t value)
    {
        return (value & WordMode) >> Shift::WordMode;
    }

    inline uint1_t GetAddressWrap16K(CRTCModeControl_t value)
    {
        return (value & AddressWrap16K) >> Shift::AddressWrap16K;
    }

    inline uint1_t GetByteMode(CRTCModeControl_t value)
    {
        return (value & ByteMode) >> Shift::ByteMode;
    }

    inline uint1_t GetHardwareReset(CRTCModeControl_t value)
    {
        return (value & HardwareReset) >> Shift::HardwareReset;
    }
    
    inline CRTCModeControl_t SetBank2Mode(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::Bank2Mode) & Bank2Mode);
    }

    inline CRTCModeControl_t SetBank4Mode(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::Bank4Mode) & Bank4Mode);
    }

    inline CRTCModeControl_t SetVerticalTotalDouble(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::VerticalTotalDouble) & VerticalTotalDouble);
    }

    inline CRTCModeControl_t SetWordMode(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::WordMode) & WordMode);
    }

    inline CRTCModeControl_t SetAddressWrap16K(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::AddressWrap16K) & AddressWrap16K);
    }

    inline CRTCModeControl_t SetByteMode(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::ByteMode) & ByteMode);
    }

    inline CRTCModeControl_t SetHardwareReset(uint1_t value)
    {
        return CRTCModeControl_t((value << Shift::HardwareReset) & HardwareReset);
    }
    
}

}}}
