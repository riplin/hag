//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

namespace Register
{

enum
{
    EndHorizontalBlank = 0x03,                              //E_H_BLNK CR3
};

}

typedef uint8_t EndHorizontalBlank_t;

namespace EndHorizontalBlank
{
    enum //Mask
    {
        EndHorizontalBlankLow = 0x1F,   //6 bit value = least significant 6 bits of the character clock counter value at which time
                                        //              horizontal blanking ends. To obtain this value, add the desired ~BLANK pulse
                                        //              width in character clocks to the Start Horizontal Blank value, which is also in character clocks.
                                        //              The 5 least significant bits of this sum are programmed into this field. The sixth bit is
                                        //              programmed into bit 7 of CR5. This allows a maximum pulse width of 63 character clocks.
                                        //              This pulse width can be extended by 64 DCLKs via bit 3 of CR5D.
        DisplaySkew = 0x60              //DSP-SKW These two bits determine the amount of display enable skew.
                                        //        Display enable skew control provides sufficient time for the CRT Controller to access the display buffer
                                        //        to obtain a character and attribute code, access the character generator font, and then go through
                                        //        the Horizontal Pixel Panning register in the Attribute Controller.
                                        //        Each access requires the display enable signal to be skewed one character clock unit so the video output
                                        //        is synchronous with the HSYNC and VSYNC signals. The bit values and amount of skew are shown in the folloing table:
                                        //        00 = Zero character clock skew
                                        //        01 = One character clock skew
                                        //        10 = Two character clock skew
                                        //        11 = Three character clock skew
    };

    namespace Shift
    {
        enum
        {
            EndHorizontalBlankLow = 0x00,
            DisplaySkew = 0x05
        };
    }

    inline EndHorizontalBlank_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::EndHorizontalBlank);
        return EndHorizontalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, EndHorizontalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::EndHorizontalBlank, CRTControllerData_t(value));
    }

}

}}}
