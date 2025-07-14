//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    DMAPadding = 0x1C54,
    DMAPaddingCommit = 0x1D54
};

}

typedef uint32_t DMAPadding_t;

namespace DMAPadding
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
