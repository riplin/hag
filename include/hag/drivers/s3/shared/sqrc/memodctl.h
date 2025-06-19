//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/s3/shared/sqrc/regtype.h>
#include <hag/drivers/vga/sqrc/memodctl.h>


namespace Hag::S3::Shared::Sequencer
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
        Unknown = 0x01,
        Unknown2 = 0x10,
        ExtendedMemoryAddress = Hag::VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress,
        SequentialAddressingMode = Hag::VGA::Sequencer::MemoryModeControl::SequentialAddressingMode,
        SelectChain4Mode = Hag::VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    };

    namespace Shift
    {
        enum
        {
            Unknown = 0x00,
            Unknown2 = 0x04,
            ExtendedMemoryAddress = Hag::VGA::Sequencer::MemoryModeControl::Shift::ExtendedMemoryAddress,
            SequentialAddressingMode = Hag::VGA::Sequencer::MemoryModeControl::Shift::SequentialAddressingMode,
            SelectChain4Mode = Hag::VGA::Sequencer::MemoryModeControl::Shift::SelectChain4Mode
        };
    }

}

}
