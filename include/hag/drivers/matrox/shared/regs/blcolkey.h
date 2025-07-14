//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    BlitColorKey = 0x1C24,
    BlitColorKeyCommit = 0x1D24
};

}

typedef uint32_t BlitColorKey_t;

namespace BlitColorKey
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
