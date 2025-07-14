//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    ForegroundColor = 0x1C24,
    ForegroundColorCommit = 0x1D24
};

}

typedef uint32_t ForegroundColor_t;

namespace ForegroundColor
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
