//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t VerticalRetraceEnd_t;

namespace VerticalRetraceEnd
{
    enum //Mask
    {
        VerticalRetraceEndCount = 0x0F,         //Value = least significant 4 bits of the scan line counter value at which VSYNC goes inactive.
                                                //        To obtain this value, add the desired VSYNC pulse width in scan line units to
                                                //        the CR10 value, also in scan line units. The 4 least significant bits of this sum are 
                                                //        programmed into this field. This allows a maximum VSYNC pulse width of 15 scan line units.
        ClearVerticalRetraceInterrupt = 0x10,   //~CLR VINT 0 = Vertical retrace interrupt cleared, 1 = The flip-flop is able to catch the next interrupt request.
                                                //        At the end of active vertical display time, a flip-flop is set for a vertical interrupt.
                                                //        The output of this flip-flop goes to the system interrupt controller. The CPU has to reset
                                                //        this flip-flop by writing a logical 0 to this bit while in the interrupt process, then set
                                                //        the bit to 1 to allow the flip-flop to catch the next interrupt request.
                                                //        Do not change the other bits in this register. This bit is cleared to 0 by the BIOS during a mode set
                                                //        and reset on power-on.
        DisableVerticalInterrupt = 0x20,        //DIS VINT 0 = Vertical retrace interrupt enabled, 1 = Vertical interrupt disabled. This bit is cleared to 0
                                                //        by the BIOS during a mode set, a reset, or power-on.
        RefreshCycleSelect = 0x40,              //REF ~3/5 0 = Three DRAM refresh cycles generated per horizontal line, 1 = Five DRAM refresh cycles
                                                //        generated per horizontal line. Selecting five refresh cycles allows use of the VGA chip with
                                                //        slow sweep rate displays (15.75 KHz). This bit is cleared to 0 by the BIOS during a mode set,
                                                //        a reset or power-on. This setting can be overridden via bits 1-0 of CR3A.
        LockWritesToCRTController = 0x80        //LOC R0-7 0 = Writing to all CRT Controller registers enabled, 1 = Writing to all bits of the CRT Controller
                                                //        registers CR0-CR7 except bit 4 of CR7 (LCM8) disabled. This bit is set to 1 by the BIOS during
                                                //        a mode set, a reset or power-on.
    };

    namespace Shift
    {
        enum
        {
            VerticalRetraceEndCount = 0x00,
            ClearVerticalRetraceInterrupt = 0x04,
            DisableVerticalInterrupt = 0x05,
            RefreshCycleSelect = 0x06,
            LockWritesToCRTController = 0x07
        };
    }

    inline VerticalRetraceEnd_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::VerticalRetraceEnd);
        return VerticalRetraceEnd_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, VerticalRetraceEnd_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::VerticalRetraceEnd, CRTControllerData_t(value));
    }

    inline uint4_t GetVerticalRetraceEndCount(VerticalRetraceEnd_t value)
    {
        return (value & VerticalRetraceEndCount) >> Shift::VerticalRetraceEndCount;
    }

    inline uint1_t GetClearVerticalRetraceInterrupt(VerticalRetraceEnd_t value)
    {
        return (value & ClearVerticalRetraceInterrupt) >> Shift::ClearVerticalRetraceInterrupt;
    }

    inline uint1_t GetDisableVerticalInterrupt(VerticalRetraceEnd_t value)
    {
        return (value & DisableVerticalInterrupt) >> Shift::DisableVerticalInterrupt;
    }

    inline uint1_t GetRefreshCycleSelect(VerticalRetraceEnd_t value)
    {
        return (value & RefreshCycleSelect) >> Shift::RefreshCycleSelect;
    }

    inline uint1_t GetLockWritesToCRTController(VerticalRetraceEnd_t value)
    {
        return (value & LockWritesToCRTController) >> Shift::LockWritesToCRTController;
    }
    
    inline VerticalRetraceEnd_t SetVerticalRetraceEndCount(uint4_t value)
    {
        return (value << Shift::VerticalRetraceEndCount) & VerticalRetraceEndCount;
    }
    
    inline VerticalRetraceEnd_t SetClearVerticalRetraceInterrupt(uint1_t value)
    {
        return (value << Shift::ClearVerticalRetraceInterrupt) & ClearVerticalRetraceInterrupt;
    }
    inline VerticalRetraceEnd_t SetDisableVerticalInterrupt(uint1_t value)
    {
        return (value << Shift::DisableVerticalInterrupt) & DisableVerticalInterrupt;
    }
    inline VerticalRetraceEnd_t SetRefreshCycleSelect(uint1_t value)
    {
        return (value << Shift::RefreshCycleSelect) & RefreshCycleSelect;
    }
    inline VerticalRetraceEnd_t SetLockWritesToCRTController(uint1_t value)
    {
        return (value << Shift::LockWritesToCRTController) & LockWritesToCRTController;
    }

    inline VerticalRetraceEnd_t Unlock(Register_t controllerIndexRegister)
    {
        VerticalRetraceEnd_t value = Read(controllerIndexRegister);
        Write(controllerIndexRegister, value & VerticalRetraceEnd_t(~LockWritesToCRTController));
        return value & VerticalRetraceEnd_t(LockWritesToCRTController);
    }

    inline void Lock(Register_t controllerIndexRegister, VerticalRetraceEnd_t previousValue = LockWritesToCRTController)
    {
        VerticalRetraceEnd_t value = Read(controllerIndexRegister);
        value &= VerticalRetraceEnd_t(~LockWritesToCRTController);
        Write(controllerIndexRegister, value | previousValue);
    }

    template <Register_t CrtControllerIndex>
    class SoftUnlock
    {
    public:
        inline SoftUnlock()
            : m_previousValue(Unlock(CrtControllerIndex))
        {
        }

        inline ~SoftUnlock()
        {
            Unlock(m_previousValue);
        }

    private:
        VerticalRetraceEnd_t m_previousValue;
    };

}

}}}
