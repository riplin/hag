//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        AGPMoveCommand = 0x014
    };
}

// agpMoveCMD starts an AGP transfer. When started agpHostAddress is loaded into the source pointer
// and agpGraphicsAddress is loaded into the destination pointer. The source pointer is incremented after
// data is fetched from AGP memory and written into frame buffer memory addresses by the destination
// pointer. The destination pointer is then incremented after the data has been written. This register is write
// only and has no default.

typedef uint32_t AGPMoveCommand_t;
namespace AGPMoveCommand
{
    enum
    {
        DestinationType = 0x00000180,
        LinearFB =          0x00000000,
        PlanarYUV =         0x00000008,
        LinearFB3D =        0x00000010,
        TexturePort =       0x00000018,
        CommandStreamID =   0x00000020,
    };

    namespace Shift
    {
        enum
        {
            DestinationType = 0x00,
            CommandStreamID = 0x05  // This bit defines which command fifo when using a host initiated
                                    // AGP data move. Default is 0x0.
        };
    }

    inline AGPMoveCommand_t Read(uint8_t* baseAddress)
    {
        return *((AGPMoveCommand_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPMoveCommand));
    }

    inline void Write(uint8_t* baseAddress, AGPMoveCommand_t value)
    {
        *((AGPMoveCommand_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPMoveCommand)) = value;
    }

}

}