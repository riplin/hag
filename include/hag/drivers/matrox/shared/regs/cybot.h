//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    ClipperYBottomBoundary = 0x1C9C,
    ClipperYBottomBoundaryCommit = 0x1D9C
};

}

typedef uint32_t ClipperYBottomBoundary_t;

namespace ClipperYBottomBoundary
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

}