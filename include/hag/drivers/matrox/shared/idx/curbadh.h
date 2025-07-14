//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    CursorBaseAddressHigh = 0x05
};

}

typedef uint8_t CursorBaseAddressHigh_t;

namespace CursorBaseAddressHigh
{
    enum //Mask
    {
        AddressHigh = 0x1F          // Cursor address high. These are the high-order bits of the cursor map address.
                                    // The 13-bit value from the high and low order cursor address locations is the base
                                    // address (bits 22:10) of the frame buffer where the cursor maps are located. The cursor
                                    // maps must be aligned on a 1 KByte boundary.
                                    // When CursorBaseAddressLow or CursorBaseAddressHigh are written, the values take effect immediately.
                                    // This may result in temporarily invalid cursor pixel values, if the cursor map is being
                                    // fetched simultaneously.
                                    // The 64 x 64 x 2 cursor map is used to define the pixel pattern within the 64 x 64 pixel
                                    // cursor window. Each pixel of the cursor is defined by two bits, referred to as bit plane
                                    // 1 and bit plane 0. The cursor data is stored in 64-bit slices in memory (each slice con-
                                    // tains all of the data for one plane of one scanline of the cursor). One plane of one scan-
                                    // line is stored in memory as follows:
                                    //
                                    // Bit:   63/62/61/ ... 32/31/ ...  2/ 1/ 0
                                    // Pixel:  0/ 1/ 2/ ... 31/32/ ... 61/62/61
                                    //
                                    // Assuming that the entire scanline of the cursor is displayed, cursor data is displayed
                                    // from bit 63 to bit 0. To facilitate fetching of cursor data from memory, slices alternate
                                    // between plane 0 and plane 1. The cursor data is organized in memory as follows:
                                    // 
                                    // 64-bit address (Q-Word)              Data
                                    //      Base + 0                    Line 0, Plane 0
                                    //      Base + 1                    Line 0, Plane 1
                                    //      Base + 2                    Line 1, Plane 0
                                    //      Base + 3                    Line 1, Plane 1
                                    //          .                             .
                                    //          .                             .
                                    //      Base + 126                  Line 63, Plane 0
                                    //      Base + 127                  Line 63, Plane 1
    };

    namespace Shift
    {
        enum
        {
            AddressHigh = 0x00
        };
    }
}

}