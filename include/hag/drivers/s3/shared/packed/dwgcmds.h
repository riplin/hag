//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/drawcmd.h>
#include <hag/drivers/s3/shared/drawcmd2.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        DrawingCommands = 0x8118,
    };
}}

namespace MMIO { namespace Packed { namespace DrawingCommands
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::DrawingCommands).ToPointer<uint32_t>();
    }

    inline void Write(DrawingCommand2_t drawingCommand2, DrawingCommand_t drawingCommand)
    {
            Get() = (uint32_t(drawingCommand2) << 16) | drawingCommand;
    }
}}}

}}}
