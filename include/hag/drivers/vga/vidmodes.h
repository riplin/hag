// Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag
{
    namespace VGA
    {

        typedef uint8_t VideoMode_t;

        namespace VideoMode
        {

            enum
            {
                DontClearDisplay = 0x80,
                T40x25x4bppG = 0x00,
                T40x25x4bppC = 0x01,
                T80x25x4bppG = 0x02,
                T80x25x4bppC = 0x03,
                G320x200x2bppC = 0x04,
                G320x200x2bppG = 0x05,
                G640x200x1bppM = 0x06,
                T80x25x1bppM = 0x07,
                Unknown1 = 0x08,
                Unknown2 = 0x09,
                Unknown3 = 0x0A,
                Reserved1 = 0x0B,
                Reserved2 = 0x0C,
                G320x200x4bppC = 0x0D,
                G640x200x4bppC = 0x0E,
                G640x350x1bppM = 0x0F,
                G640x350x4bppC = 0x10,
                G640x480x1bppM = 0x11,
                G640x480x4bppC = 0x12,
                G320x200x8bppC = 0x13,

                MaxValid = G320x200x8bppC
            };

        }

    }
}
