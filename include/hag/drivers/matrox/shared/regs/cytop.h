//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    ClipperYTopBoundary = 0x1C98,
    ClipperYTopBoundaryCommit = 0x1D98
};

}

typedef uint32_t ClipperYTopBoundary_t;

namespace ClipperYTopBoundary
{

    enum //Mask
    {
        Value = 0x007FFFFF
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