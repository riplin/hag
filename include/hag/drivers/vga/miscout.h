//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace VGA
{

typedef uint8_t MiscellaneousOutput_t;

namespace MiscellaneousOutput
{
    enum //Mask
    {
        IOAddressSelect = 0x1,                              //IOA SEL 0 = Monochrome, 1 = Color
        EnableCPUDisplayMemoryAccess = 0x02,                //ENB RAM 0 = Disable, 1 = Enable
        ClockSelect = 0x0C,                                 //00 = 25.175MHz DCLK for 640 horizontal pixels
                                                            //11 = 28.322MHz DCLK for 720 horizontal pixels
                                                            //10 = Reserved
                                                            //11 = Enables loading of DCLK PLL parameters in SR12 and SR13.
        PageSelect = 0x20,                                  //PGSL Select High 64K Page, 0 = low 64k page, 1 = high 64k page
        SelectNegativeHorizontalSyncPulse = 0x40,           //~HSP 0 = Select a positive horizontal retrace sync pulse, 1 = Select negative...
        SelectNegativeVerticalSyncPulse = 0x80              //~VSP 0 = Select a positive vertical retrace sync pulse, 1 = Select negative...
    };

    namespace Shift
    {
        enum
        {
            IOAddressSelect = 0x00,
            EnableCPUDisplayMemoryAccess = 0x01,
            ClockSelect = 0x02,
            PageSelect = 0x05,
            SelectNegativeHorizontalSyncPulse = 0x06,
            SelectNegativeVerticalSyncPulse = 0x07
        };
    }

    inline MiscellaneousOutput_t Read()
    {
        return MiscellaneousOutput_t(SYS_ReadPortByte(Register::MiscellaneousR));
    }
    
    inline void Write(MiscellaneousOutput_t value)
    {
        SYS_WritePortByte(Register::MiscellaneousW, value);
    }

    inline uint1_t GetIOAddressSelect(MiscellaneousOutput_t value)
    {
        return (value & IOAddressSelect) >> Shift::IOAddressSelect;
    }
    
    inline uint1_t GetEnableCPUDisplayMemoryAddress(MiscellaneousOutput_t value)
    {
        return (value & EnableCPUDisplayMemoryAccess) >> Shift::EnableCPUDisplayMemoryAccess;
    }
    
    inline uint2_t GetClockSelect(MiscellaneousOutput_t value)
    {
        return (value & ClockSelect) >> Shift::ClockSelect;
    }
    
    inline uint1_t GetPageSelect(MiscellaneousOutput_t value)
    {
        return (value & PageSelect) >> Shift::PageSelect;
    }
    
    inline uint1_t GetSelectNegativeHorizontalSyncPulse(MiscellaneousOutput_t value)
    {
        return (value & SelectNegativeHorizontalSyncPulse) >> Shift::SelectNegativeHorizontalSyncPulse;
    }
    
    inline uint1_t GetSelectNegativeVerticalSyncPulse(MiscellaneousOutput_t value)
    {
        return (value & SelectNegativeVerticalSyncPulse) >> Shift::SelectNegativeVerticalSyncPulse;
    }
    
    inline MiscellaneousOutput_t SetIOAddressSelect(uint1_t value)
    {
        return MiscellaneousOutput_t((value << Shift::IOAddressSelect) & IOAddressSelect);
    }
    
    inline MiscellaneousOutput_t SetEnableCPUDisplayMemoryAddress(uint1_t value)
    {
        return MiscellaneousOutput_t((value << Shift::EnableCPUDisplayMemoryAccess) & EnableCPUDisplayMemoryAccess);
    }
    
    inline MiscellaneousOutput_t SetClockSelect(uint2_t value)
    {
        return MiscellaneousOutput_t((value << Shift::ClockSelect) & ClockSelect);
    }
    
    inline MiscellaneousOutput_t SetPageSelect(uint1_t value)
    {
        return MiscellaneousOutput_t((value << Shift::PageSelect) & PageSelect);
    }
    
    inline MiscellaneousOutput_t SetSelectNegativeHorizontalSyncPulse(uint1_t value)
    {
        return MiscellaneousOutput_t((value << Shift::SelectNegativeHorizontalSyncPulse) & SelectNegativeHorizontalSyncPulse);
    }
    
    inline MiscellaneousOutput_t SetSelectNegativeVerticalSyncPulse(uint1_t value)
    {
        return MiscellaneousOutput_t((value << Shift::SelectNegativeVerticalSyncPulse) & SelectNegativeVerticalSyncPulse);
    }

}

}}
