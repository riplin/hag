//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    ClipperXLeftBoundary = 0x1CA0,
    ClipperXLeftBoundaryCommit = 0x1DA0
};

}

typedef uint32_t ClipperXLeftBoundary_t;

namespace ClipperXLeftBoundary
{

    enum //Mask
    {
        Value = 0x000007FF
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
