//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/attribc/data.h>

namespace Hag { namespace VGA { namespace AttributeController
{

typedef uint8_t HorizontalPixelPanning_t;

//This register specifies the number of pixels to shift the display data horizontally
//to the left. Pixel panning is available in both text and graphics modes. It is not
//available with Enhanced mode memroy mappings (CR31_3 = 1).

namespace HorizontalPixelPanning
{
    enum //Mask
    {
        NumberOfPanShift = 0x0F //This register selects the number of pixels to shift
                                //the display data horizontally to the left. In the 9
                                //pixels/character text mode, the output can be shifted
                                //a maximum shift of 8 pixels. In the 8 pixels/character
                                //text mode and all graphics modes, except 256 color
                                //mode, a maximum shift of 7 pixels is possible. In the
                                //256 color mode, bit 0 of this register must be 0
                                //resulting in only 4 panning positions per display byte.
                                //The panning is controlled as follows:
                                //Bits 3-0          Number of pixels shifted in
                                //          9 pixel/char    8 pixel/char    256 color mode
                                //0000          1               0               0
                                //0001          2               1               -
                                //0010          3               2               1
                                //0011          4               3               -
                                //0100          5               4               2
                                //0101          6               5               -
                                //0110          7               6               3
                                //0111          8               7               -
                                //1000          0               -               -
    };

    namespace Shift
    {
        enum
        {
            NumberOfPanShift = 0x00
        };
    }

    inline HorizontalPixelPanning_t Read()
    {
        AttributeControllerIndex::Write(AttributeControllerRegister::HorizontalPixelPanning);
        return HorizontalPixelPanning_t(AttributeControllerData::Read());
    }
    
    inline void Write(HorizontalPixelPanning_t value)
    {
        AttributeControllerData::Write(AttributeControllerRegister::HorizontalPixelPanning, AttributeControllerData_t(value));
    }

    inline uint4_t GetNumberOfPanShift(HorizontalPixelPanning_t value)
    {
        return (value & NumberOfPanShift) >> Shift::NumberOfPanShift;
    }

    inline HorizontalPixelPanning_t SetNumberOfPanShift(uint4_t value)
    {
        return HorizontalPixelPanning_t(value << Shift::NumberOfPanShift) & NumberOfPanShift;
    }
}

}}}
