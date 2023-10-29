//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace S3 { namespace ReadRegister
{

    enum
    {
        MinorAxisPixelCount = 0x00,                         //MIN_AXIS_PCNT
        TopScissors = 0x01,                                 //SCISSORS_T
        LeftScissors = 0x02,                                //SCISSORS_L
        BottomScissors = 0x03,                              //SCISSORS_B
        RightScissors = 0x04,                               //SCISSORS_R
        PixelControl = 0x0A,                                //PIX_CNTL
        MultifunctionControlMiscellaneous2 = 0x0D,          //MULT_MISC2
        MultifunctionControlMiscellaneous = 0x0E,           //MULT_MISC
        ReadRegisterSelect = 0x0F,                          //READ_SEL
    };

}}}
