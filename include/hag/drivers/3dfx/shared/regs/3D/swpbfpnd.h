//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        SwapBufferPending = 0x24c
    };
}

// swapBufferPend Register
// Writes to the swapBufferPend register increments the swap buffer pending count of the Banshee status
// register. Writes take effect immediately and are available only through direct access.

// NOTE (swapBufferPend considered harmful):

// Writes to this register during command FIFO operation on Banshee have been known to end up in the
// linear frame buffer instead of the register. This can cause hole counting issues, and should therefore be
// avoided. The following method should be used instead:
//     • Decide the maximum number of pending swaps to allow (7 is very high).
//     • create an area to store the location in the command FIFO of all swapBufferCMDs
//     • when a swapBufferCMD is placed in the command FIFO, store its location (after finding a
//       free slot), and increment a local (software) swap pending count.
//     • whenever a read of the readPtr register occurs do the following:
//         Check to see if the read pointer is less than it was the last time it was checked (the FIFO has
//         wrapped). If it is, then traverse the list of stored swapBufferCMD locations, for each one
//         that is less than the read pointer, mark the storage slot as free and decrement the pending
//         swap count.

//         If the read pointer is greater than the last time it was checked, traverse the list of stored
//         swapBufferCMD locations, and if they are between the last check and the read pointer, mark
//         the slot as free and decrement the swap count.

//         Store the value of the read pointer for use later (see above).

// Note that this process must be followed on all reads of the command FIFO read pointer--including those
// at the time the command FIFO is wrapped.

typedef uint32_t SwapBufferPending_t;
namespace SwapBufferPending
{
    enum
    {
        Value = 0xffffffff
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