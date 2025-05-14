//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    BlitColorMask = 0x1C20,
    BlitColorMaskCommit = 0x1D20
};

}

typedef uint32_t BlitColorMask_t;

namespace BlitColorMask
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

}}}
