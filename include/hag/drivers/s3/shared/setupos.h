//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    SetupOptionSelect = 0x102,                              //SETUP_MD
};

}

typedef uint8_t SetupOptionSelect_t;

namespace SetupOptionSelect
{
    enum
    {
        DisableSleepMode = 0x01,        //When in setup mode (I/O address 46E8H, bit 4 =1), the Trio32/Trio64
                                        //responds to a single option select byte at I/O address 102H and treats
                                        //this bit as the Video Subsystem sleep bit.
                                        //0 = Tri032/Trio64 does not respond to commands, addresses, or data on the data bus.
                                        //    If the Tri032/Tri064 was set up and is generating video output when this bit is
                                        //    cleared to 0, the output is still generated
                                        //1 = Trio32/Trio64 responds to commands, addresses, or data on the data bus
                                        //The Tri032/Tri064 responds only to address 102H when in the setup mode.
                                        //No other addresses are valid at that time. The Tri032/Tri064 ignores address 102H
                                        //when in the enabled mode (I/O address 46E8H, bit 4 = 0), and decodes normal I/O and
                                        //memory addresses.
                                        //If bit 2 of CR65 is set to 1, 3C3H is the address used for setup instead of 46E8H.
    };

    inline SetupOptionSelect_t Read()
    {
        return SetupOptionSelect_t(SYS_ReadPortByte(Register::SetupOptionSelect));
    }

    inline void Write(SetupOptionSelect_t value)
    {
        SYS_WritePortByte(Register::SetupOptionSelect, value);
    }
}

}
