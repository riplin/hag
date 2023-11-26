//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/shared/sqrc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace Sequencer
{

namespace Register
{
    
enum
{
    UnlockExtendedSequencer = 0x08,         //UNLK_EXSR SR8
};

}

typedef uint8_t UnlockExtendedSequencer_t;

namespace UnlockExtendedSequencer
{
    
    enum //Mask
    {
        UnlockKey = 0x0F,       //Writing 0x06 unlocks extended Sequencer registers. Writing 0x00 locks them.
        UnlockCode = 0x06,
        LockCode = 0x00
    };

    namespace Shift
    {
        enum
        {
            UnlockKey = 0x00
        };
    }

    inline UnlockExtendedSequencer_t Read()
    {
        VGA::SequencerIndex::Write(Register::UnlockExtendedSequencer);
        return UnlockExtendedSequencer_t(VGA::SequencerData::Read());
    }
    
    inline void Write(UnlockExtendedSequencer_t value)
    {
        VGA::SequencerData::Write(Register::UnlockExtendedSequencer, VGA::SequencerData_t(value));
    }

    inline void Unlock()
    {
        Write(UnlockCode);
    }
    
    inline void Lock(UnlockExtendedSequencer_t previousValue = LockCode)
    {
        Write(previousValue);
    }
    
    class SoftUnlock
    {
    public:
        inline SoftUnlock()
            : m_previousValue(Read())
        {
            Unlock();
        }

        inline ~SoftUnlock()
        {
            Lock(m_previousValue);
        }

    private:
        UnlockExtendedSequencer_t m_previousValue;
    };
}

}}}}
