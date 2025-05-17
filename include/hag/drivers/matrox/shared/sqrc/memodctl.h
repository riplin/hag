//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/memodctl.h>


namespace Hag { namespace Matrox { namespace Shared { namespace Sequencer
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
    using namespace VGA::Sequencer::MemoryModeControl;

    enum //Mask
    {
        Unknown1 = 0x01
    };

    namespace Shift
    {
        using namespace VGA::Sequencer::MemoryModeControl::Shift;

        enum
        {
            Unknown1 = 0x00,
        };
    }
}

}}}}
