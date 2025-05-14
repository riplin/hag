//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    MemoryControlWaitState = 0x1C08,
    MemoryControlWaitStateCommit = 0x1D08
};

}

typedef uint32_t MemoryControlWaitState_t;

namespace MemoryControlWaitState
{

    enum //Mask
    {
        CASLatency =            0x00000001,     // CAS latency. This bit must be programmed prior to the memory power-up sequence.
                                                // Refer to ‘Power Up Sequence’ on page 5-20 for more details.
        CASLatency2Cycles =     0x00000000,     // 2 cycle latency
        CASLatency3Cycles =     0x00000001,     // 3 cycle latency
        RAStoCASDelay =         0x00000100,     // RAS to CAS delay. This bit selects one of two RAS to CAS delay values, as shown below:
        RAStoCASDelay2Cycles =  0x00000000,     // 2 cycle delay 
        RAStoCASDelay3Cycles =  0x00000100,     // 3 cycle delay 
        RASMinimumActiveTime =  0x00030000,     // RAS minimum active time. The valid values are shown below:
        RASMinumum4Cycles =     0x00000000,     // 4 cycle active
        RASMinumum5Cycles =     0x00010000,     // 5 cycle active
        RASMinumum6Cycles =     0x00020000,     // 6 cycle active
        RASMinumum7Cycles =     0x00030000      // 7 cycle active
    };

    namespace Shift
    {
        enum
        {
            CASLatency = 0x00,
            RAStoCASDelay = 0x08,
            RASMinimumActiveTime = 0x010
        };
    }
}

}}}
