//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    ClipperXBoundary = 0x1C80,
    ClipperXBoundaryCommit = 0x1D80
};

}

typedef uint32_t ClipperXBoundary_t;

// The ClipperXBoundary register is not a physical register. It is simply an alternate way to load
// the ClipperXRightBoundary and ClipperXLeftBoundary registers.

namespace ClipperXBoundary
{

    enum //Mask
    {
        Left = 0x000007FF,      // Clipper x left boundary. See the ClipperXLeftBoundary register.
        Right = 0x07FF0000      // Clipper x right boundary. See the ClipperXRightBoundary register.
    };

    namespace Shift
    {
        enum
        {
            Left = 0x00,
            Right = 0x10
        };
    }
}

}}}
