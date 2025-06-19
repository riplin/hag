//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MemoryOrigin = 0x1C94,
    MemoryOriginCommit = 0x1D94
};

}

typedef uint32_t MemoryOrigin_t;

namespace MemoryOrigin
{

    enum //Mask
    {
        YOrigin = 0x007FFFFF
    };

    namespace Shift
    {
        enum
        {
            YOrigin = 0x00
        };
    }
}

}
