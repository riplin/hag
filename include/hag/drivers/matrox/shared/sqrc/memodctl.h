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
        Unknown1 = 0x01,
        ExtendedMemoryAddress = VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress,
        SequentialAddressingMode = VGA::Sequencer::MemoryModeControl::SequentialAddressingMode,
        SelectChain4Mode = VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    };

    namespace Shift
    {
        using namespace VGA::Sequencer::MemoryModeControl::Shift;

        enum
        {
            Unknown1 = 0x00,
            ExtendedMemoryAddress = VGA::Sequencer::MemoryModeControl::Shift::ExtendedMemoryAddress,
            SequentialAddressingMode = VGA::Sequencer::MemoryModeControl::Shift::SequentialAddressingMode,
            SelectChain4Mode = VGA::Sequencer::MemoryModeControl::Shift::SelectChain4Mode
        };
    }
}

}}}}
