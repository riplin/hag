//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/s3/shared/sqrc/regtype.h>
#include <hag/drivers/vga/sqrc/memodctl.h>


namespace Hag { namespace S3 { namespace Shared { namespace Sequencer
{

namespace Register
{
    
enum
{
    MemoryModeControl = VGA::Sequencer::Register::MemoryModeControl
};

}

typedef VGA::Sequencer::MemoryModeControl_t MemoryModeControl_t;

namespace MemoryModeControl
{
    using namespace Hag::VGA::Sequencer::MemoryModeControl;

    enum //Mask
    {
        Unknown = 0x01,
        Unknown2 = 0x10
    };

    namespace Shift
    {
        using namespace Hag::VGA::Sequencer::MemoryModeControl::Shift;

        enum
        {
            Unknown = 0x00,
            Unknown2 = 0x04
        };
    }

}

}}}}
