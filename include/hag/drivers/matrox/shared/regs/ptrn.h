//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    Pattern0 = 0x1C10,
    Pattern0Commit = 0x1D10,
    Pattern1 = 0x1C14,
    Pattern1Commit = 0x1D14
};

}

typedef uint32_t Pattern_t;

namespace Pattern
{

    enum //Mask
    {
        Value = 0xFFFFFFFF
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

}
