//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    ClipperXRightBoundary = 0x1CA4,
    ClipperXRightBoundaryCommit = 0x1DA4
};

}

typedef uint32_t ClipperXRightBoundary_t;

namespace ClipperXRightBoundary
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

}
